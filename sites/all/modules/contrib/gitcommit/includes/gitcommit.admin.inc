<?php

/**
 * @file
 * Provides administrative interfaces for Git Commit Webhook.
 */

/**
 * Form builder for settings page.
 */
function gitcommit_settings_form($form, &$form_state) {
  $form['webhook'] = array(
    '#title' => t('Webhooks'),
    '#type' => 'fieldset',
  );

  $form['webhook']['gitcommit_webhook_enabled'] = array(
    '#title' => t('Enable webhooks'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('gitcommit_webhook_enabled', FALSE),
  );

  $form['webhook']['gitcommit_webhook_repo'] = array(
    '#title' => t('Create entity from Repository'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => array(
      'beanstalk' => t('Beanstalk'),
      'bitbucket' => t('Bitbucket'),
      'github' => t('GitHub'),
    ),
    '#default_value' => variable_get('gitcommit_webhook_repo', ''),
  );

  $form['webhook']['gitcommit_webhook_security_token'] = array(
    '#title' => t('Webhook security token'),
    '#type' => 'textfield',
    '#default_value' => variable_get('gitcommit_webhook_security_token', ''),
    '#description' => t('If set, a "token" parameter will be required in URL, e.g. http://mywebsite.com/gitcommit-webhook/github?token=1234'),
  );

  return system_settings_form($form);
}

/**
 * Form builder for clear commit history page.
 */
function gitcommit_clear_history_form($form, &$form_state) {
  if (isset($form_state['storage']['delete_time'])) {
    $args = array(
      '@time' => format_date($form_state['storage']['delete_time'], 'short'),
    );

    $question = t('Are you sure you want to delete all commits performed before @time?', $args);
    $form = confirm_form($form, $question, current_path());
  }
  else {
    $form['delete_time'] = array(
      '#title' => t('Clear commits performed before:'),
      '#type' => 'textfield',
      '#description' => t('Enter an !epoch.', array('!epoch' => l(t('epoch time'), 'https://en.wikipedia.org/wiki/Unix_time'))),
      '#element_validate' => array('element_validate_integer'),
      '#required' => TRUE,
      '#default_value' => '0',
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Clear history'),
    );
  }

  return $form;
}

/**
 * Submit handler for gitcommit_clear_history_form.
 */
function gitcommit_clear_history_form_submit($form, &$form_state) {
  if (isset($form_state['storage']['delete_time'])) {
    $cids = db_select('gitcommit', 'g')->fields('g', array('cid'))
      ->condition('time', strtotime($form_state['storage']['delete_time']), '<')
      ->execute()
      ->fetchCol();

    gitcommit_delete_multiple($cids);
    drupal_set_message(format_plural(sizeof($cids), '1 commit has been deleted.', '@count commits have been deleted.'));
  }
  else {
    // Set confirmation form.
    $form_state['storage']['delete_time'] = $form_state['values']['delete_time'];
    $form_state['rebuild'] = TRUE;
  }
}
