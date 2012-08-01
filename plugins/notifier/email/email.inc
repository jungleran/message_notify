<?php

$plugin = array(
  'title' => t('Email'),
  'description' => t('Send Message via email.'),
  'class' => 'MessageNotifierEmail',
  'defaults' => array(
    // Override mail. Don't use the email that is assigned to the user.
    'mail' => FALSE,
  ),
);
