<?php

/**
 * @file
 * Hooks provided by Git Commit Webhook.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Acts on incoming commits data.
 *
 * @param array $data
 *   Incoming repository event data.
 */
function hook_gitcommit_webhook($data) {
  if (!empty($data['commits'])) {
    watchdog('gitcommit', 'Incoming data: !data', array('!data' => print_R($data, TRUE)));
  }
}

/**
 * Control access to a git commit.
 *
 * @param array $commit
 *   The git commit object.
 * @param string $op
 *   Operation to be performed. Possible values:
 *   - "create"
 *   - "delete"
 *   - "update"
 *   - "view"
 * @param $account
 *   The user object to perform the access check operation on.

 * @return
 *   - TRUE to allow the operation.
 *   - FALSE to deny the operation.
 *   - Return nothing to not affect this operation at all.
 */
function hook_gitcommit_access($commit, $op, $account) {
  if ($op == 'update' && user_access('administer content')) {
    return TRUE;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
