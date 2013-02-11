<?php

if (module_exists('sms')) {
  $plugin = array(
    'title' => t('SMS'),
    'description' => t('Send SMS using the SMS framework module.'),
    'class' => 'MessageNotifierSMS'
  );
}

