<?php

/**
 * Test notifier.
 */
class MessageNotifierTest extends MessageNotifierBase {

  /**
   * Add Message notify view mode.
   */
  public static function viewModes() {
    return array(
      'message_notify_foo' => array('label' => t('View mode foo')),
      'message_notify_bar' => array('label' => t('View mode bar')),
    );
  }

  public function deliver(array $output = array()) {
    return TRUE;
  }

}
