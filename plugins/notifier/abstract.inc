<?php

/**
 * Additional behaviors for a Entity Reference field.
 *
 * Implementations that wish to provide an implementation of this should
 * register it using CTools' plugin system.
 */
interface MessageNotifierInterface {

  /**
   * Constructor for the notifier.
   *
   * @param $notifier_name
   *   The name of the notifier plugin.
   * @param Message $message
   *   The Message entity.
   */
  public function __construct($notifier_name, Message $message);

  /**
   * Entry point to send and process a message.
   */
  public function send();

  /**
   * Deliver a message via the required transport method.
   *
   * @param $output
   *   Array keyed by the view mode, and the rendered entity in the
   *   specified view mode.
   *
   * @return
   *   TRUE or FALSE based on delivery status.
   */
  public function deliver(array $output = array());



  /**
   * Post send operations.
   */
  public function postSend($result);

  /**
   * Post send operations.
   */
  public function postSend($result, array $output = array());

  /**
   * Determine if user can access notifier.
   */
  public function access();

  /**
   * Allow notifier to define its own view modes.
   *
   * Those view modes are later going to be rendered and sent.
   */
  public function viewModes();
}

/**
 * An abstract implementation of MessageNotifierInterface.
 */
abstract class MessageNotifierBase implements MessageNotifierInterface {

  /**
   * The plugin definition.
   */
  protected $plugin;

  /**
   * The message entity.
   */
  protected $message;

  public function __construct($notifier_name, Message $message) {
    $this->plugin = message_notify_get_notifier($notifier_name);
    $this->message = $message;
  }

  public function viewModes() {
    return array();
  }

  public function send() {
    $message = $this->message;
    $output = array();
    foreach ($this->viewModes() as $view_mode) {
      $output[$view_mode] = $message->view($view_mode);
    }
    $result = $this->deliver($output);
    $this->postSend($result, $output);
  }

  public function deliver($output) {}

  /**
   * Act upon send result.
   *
   * - Save the rendered messages if needed.
   * - Invoke watchdog error on failure.
   */
  public function postSend($result, $output) {
    $plugin = $this->plugin;
    $message = $this->message;

    $save = FALSE;
    if (!$result) {
      watchdog('message_notify', t('Could not send message using @title to user ID @uid.'), array('@label' => $plugin['title'], '@uid' => $message->uid), WATCHDOG_ERROR);
      if ($plugin['save on fail']) {
        $save = TRUE;
      }
    }
    elseif ($result && $plugin['save on success']) {
      $save = TRUE;
    }

    if (!$save) {
      return;
    }

    if ($plugin['rendered fields']) {
      // Save the rendered output into matching fields.
      $wrapper = entity_metadata_wrapper('message', $message);
      foreach ($this->viewModes() as $view_mode) {
        if (empty($plugin['rendered fields'][$view_mode])) {
          throw new MessageNotifyException(format_string('The rendered view mode @mode cannot be saved to field, as there is not a matching one.', array('@mode' => $view_mode)));
        }
        $field_name = $plugin['rendered fields'][$view_mode];

        // Get the format from the field. We assume the first delta is the
        // same as the rest.
        if (empty($wrapper->{$options['rendered body field']}->format)) {
          $wrapper->{$field_name}->set($output[$view_mode]);
        }
        else {
          $format = $wrapper->type->{MESSAGE_FIELD_MESSAGE_TEXT}->get(0)->format->value();
          $wrapper->{$field_name}->set(array('value' => $output[$view_mode], 'format' => $format));
        }
      }
    }

    if ($save) {
      $message->save();
    }
  }

  public function access() {
    return TRUE;
  }

}
