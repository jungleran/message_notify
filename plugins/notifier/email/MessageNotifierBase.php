<?php

/**
 * Email notifier.
 */
class MessageNotifierEmail implements MessageNotifierBase {

  /**
   * Add Message notify view mode.
   */
  public function viewModes() {
    return array(
      'message_notify_email_subject' => t('Notify - Email subject'),
      'message_notify_email_body' => t('Notify - Email subject'),
    );
  }

  public function deliver(array $output = array()) {
    $plugin = $this->plugin;
    $message = $this->message;

    $account = user_load($message->uid);
    $mail = $plugin['mail'] ? $plugin['mail'] : $account->mail;

    $languages = language_list();
    if (!$plugin['language override']) {
      $lang = !empty($account->language) && $account->language != LANGUAGE_NONE ? $languages[$account->language]: language_default();
    }
    else {
      $lang = $languages[$message->language];
    }

    return drupal_mail('message_notify', $message->type, $mail, $lang, $output);
  }

}
