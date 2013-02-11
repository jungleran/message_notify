<?php

/**
 * @file
 * SMS notifier.
 */

class MessageNotifierSMS {

  public static function viewModes() {
    return array(
      'message_notify_sms_body' => array('label' => t('Notify - SMS Body')),
    );
  }

  public function deliver(array $output = array()) {
    if (!isset($this->message->smsNumber)) {
      throw new MessageNotifyException('Message cannot be sent using SMS as the "smsNumber" property is missing from the Message entity.');
    }
    return sms_send($this->message->smsNumber, $output['message_notify_body']);
  }
}