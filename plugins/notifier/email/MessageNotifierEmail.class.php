<?php

/**
 * Email notifier.
 */
class MessageNotifierEmail extends MessageNotifierBase {

  /**
   * Add Message notify view mode.
   */
  public static function viewModes() {
    return array(
      'message_notify_email_subject' => array('label' => t('Notify - Email subject')),
      'message_notify_email_body' => array('label' => t('Notify - Email subject')),
    );
  }

  public function deliver(array $output = array()) {
    $plugin = $this->plugin;
    $message = $this->message;

    $options = $plugin['options'];

    $account = user_load($message->uid);
    $mail = $options['mail'] ? $options['mail'] : $account->mail;

    $languages = language_list();
    if (!$options['language override']) {
      $lang = !empty($account->language) && $account->language != LANGUAGE_NONE ? $languages[$account->language]: language_default();
    }
    else {
      $lang = $languages[$message->language];
    }

    return drupal_mail('message_notify', $message->type, $mail, $lang, $output);
  }

}
