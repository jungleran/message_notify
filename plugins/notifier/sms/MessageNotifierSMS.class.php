<?php

/**
 * @file
 * SMS notifier.
 */

class MessageNotifierSMS {

  public function deliver(array $output = array()) {
    if (!isset($this->message->smsNumber)) {
      throw new MessageNotifyException('Message cannot be sent using SMS as the "smsNumber" property is missing from the Message entity.');
    }
    return sms_send($this->phonenumber,  $output['message_notify_body']);
  }
}