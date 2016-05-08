<?php

/**
 * @file
 * Webhooks listener.
 */

/**
 * Webhook Listener callback.
 */
function gitcommit_webhook_listener() {
  // Check for security token.
  if ($token = variable_get('gitcommit_webhook_security_token', '')) {
    $params = drupal_get_query_parameters();

    if (!isset($params['token']) || $params['token'] != $token) {
      $msg = 'Invalid security token. Access denied.';
      watchdog('gitcommit', $msg, array(), WATCHDOG_WARNING);

      return array(
        'success' => FALSE,
        'message' => $msg,
      );
    }
  }

  try {
    $data = isset($_POST['payload']) ? $_POST['payload'] : file_get_contents('php://input');

    if (!empty($data)) {
      $data = drupal_json_decode($data);

      // Allowing other modules to interact here.
      module_invoke_all('gitcommit_webhook', $data);

      if ($repo = variable_get('gitcommit_webhook_repo', '')) {
        module_load_include('inc', 'gitcommit', 'includes/repos/gitcommit.' . $repo);

        $function = 'gitcommit_' . $repo . '_processor';
        $commits = $function($data);
      }

      $count = empty($commits) ? 0 : count($commits);

      return array(
        'success' => TRUE,
        'message' => count($commits) . ' commit(s) created.',
      );
    }
  }
  catch (Exception $e) {
    watchdog_exception('gitcommit', $e);

    return array(
      'success' => FALSE,
      'message' => $e->getMessage(),
    );
  }
}
