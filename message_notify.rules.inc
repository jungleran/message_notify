<?php

/**
 * @file
 * Rules integration for the Message notify module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function message_notify_rules_action_info() {
  $items = array();
  $items['message_notify_process'] = array(
    'label' => t('Send Message with Message notify'),
    'group' => t('Message notify'),
    'parameter' => array(
      'message' => array(
        'type' => 'message',
        'label' => t('Message'),
        'description' => t('The message to be processed and sent using Message notify.'),
      ),
      'save_on_fail' => array(
        'type' => 'boolean',
        'label' => t('Save on fail'),
        'description' => t('Save message if sending failed.'),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
      'save_on_success' => array(
        'type' => 'boolean',
        'label' => t('Save on success'),
        'description' => t('Save message if sending successed.'),
        'default value' => TRUE,
        'optional' => TRUE,
        'restriction' => 'input',
      ),
      'rendered_subject_field' => array(
        'type' => 'token',
        'label' => t('Rendered subject field'),
        'description' => t('The field to save the rendered subject.'),
        'default value' => FALSE,
        'optional' => TRUE,
        'options list' => 'message_notify_field_text_list',
      ),
      'rendered_body_field' => array(
        'type' => 'token',
        'label' => t('Body subject field'),
        'description' => t('The field to save the rendered body.'),
        'default value' => FALSE,
        'optional' => TRUE,
        'options list' => 'message_notify_field_text_list',
      ),
    ),
    'base' => 'message_notify_rules_process',
  );

  return $items;
}

/**
 * Action: Process and send Message.
 */
function message_notify_rules_process(Message $message, $save_on_fail, $save_on_success, $rendered_subject_field, $rendered_body_field) {
  $options = array(
    'save on fail'           => $save_on_fail,
    'save on success'        => $save_on_success,
    'rendered subject field' => $rendered_subject_field,
    'rendered body field'    => $rendered_body_field,
  );
  message_notify_send_mail($message, $options);
}

/**
 * Options list; Return the text field attached to the selected message
 */
function message_notify_field_text_list() {
  $options = array(FALSE => '- ' . t('None') . ' -');


  foreach (field_info_instances('message') as $message_type => $instances) {
    foreach ($instances as $field_name => $instance) {
      if (!empty($options[$field_name])) {
        // Field is already in the options array.
        continue;
      }
      $field = field_info_field($field_name);
      if (!in_array($field['type'], array('text', 'text_long', 'text_with_summary'))) {
        // Field is not a text field.
        continue;
      }
      $options[$field_name] = $instance['label'];
    }
  }

  return $options;
}

/**
 * @}
 */
