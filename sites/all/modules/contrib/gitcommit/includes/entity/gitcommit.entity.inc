<?php

/**
 * @file
 * Provides hook implementations and functions related to Entity API module.
 */

/**
 * Implements hook_entity_info().
 */
function gitcommit_entity_info() {
  $info = array();

  $info['gitcommit'] = array(
    'label' => t('Commit'),
    'plural label' => t('Commits'),
    'base table' => 'gitcommit',
    'entity class' => 'GitCommit',
    'controller class' => 'EntityAPIController',
    'entity keys' => array(
      'id' => 'cid',
      'label' => 'message',
    ),
    'module' => 'gitcommit',
    'label callback' => 'entity_class_label',
    'access callback' => 'gitcommit_access',
    'fieldable' => TRUE,
  );

  return $info;
}

/**
 * Implements hook_entity_property_info_alter().
 */
function gitcommit_entity_property_info_alter(&$info) {
  $properties = &$info['gitcommit']['properties'];
  $custom = array();

  $custom['hash'] = array(
    'label' => t('Hash ID'),
    'description' => t('The hash ID of the commit'),
  );

  $custom['message'] = array(
    'label' => t('Message'),
    'description' => t('The commit message.'),
  );

  $custom['time'] = array(
    'label' => t('Time'),
    'type' => 'date',
    'description' => t('Time when the commit happened.'),
  );

  $custom['url'] = array(
    'label' => t('URL'),
    'type' => 'uri',
    'description' => t('The URL of the commit.'),
  );

  $custom['author_mail'] = array(
    'label' => t('Author e-mail'),
    'description' => t('The e-mail address of the commit author.'),
  );

  $custom['author_name'] = array(
    'label' => t('Author name'),
    'description' => t('The name of the commit author.'),
  );

  $custom['repository_name'] = array(
    'label' => t('Repository name'),
    'description' => t('The repository name.'),
  );

  $custom['repository_url'] = array(
    'label' => t('Repository URL'),
    'type' => 'uri',
    'description' => t('The URL of the repository.'),
  );

  foreach ($custom as $property_id => $custom_values) {
    $properties[$property_id] = array(
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer git commit',
    ) + $custom_values + $properties[$property_id];
  }
}

/**
 * Create a new git commit object.
 *
 * @param $values
 *   An array of values to set, keyed by property name.
 * @return
 *   A new git commit instance.
 *
 * @see entity_create()
 */
function gitcommit_create($values) {
  return entity_create('gitcommit', $values);
}

/**
 * Fetch a git commit object.
 *
 * @param $cid
 *   Integer specifying the commit id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $commit object or FALSE if it cannot be loaded.
 *
 * @see gitcommit_load_multiple()
 */
function gitcommit_load($cid, $reset = FALSE) {
  $commits = gitcommit_load_multiple(array($cid), array(), $reset);
  return reset($commits);
}

/**
 * Load multiple git commits based on certain conditions.
 *
 * @param $cids
 *   An array of commit IDs.
 * @param $conditions
 *   An array of conditions to match against the {gitcommit} table.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of commit objects, indexed by pid.
 *
 * @see entity_load()
 * @see gitcommit load()
 */
function gitcommit_load_multiple($cids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('gitcommit', $cids, $conditions, $reset);
}

/**
 * Deletes a git commit.
 *
 * @param $commit
 *   The git commit object.
 *
 * @see entity_delete()
 */
function gitcommit_delete($commit) {
  entity_delete('gitcommit', entity_id('gitcommit', $commit));
}

/**
 * Delete multiple git commits.
 *
 * @param $cids
 *   An array of commit IDs.
 *
 * @see entity_delete_multiple()
 */
function gitcommit_delete_multiple($cids) {
  entity_delete_multiple('gitcommit', $cids);
}

/**
 * Saves a git commit to the database.
 *
 * @param $commit
 *   The git commit object.
 *
 * @see entity_save()
 */
function gitcommit_save($commit) {
  entity_save('gitcommit', $commit);
}

/**
 * Determines whether the given user has access to a git commit.
 *
 * @param $op
 *   The operation being performed. One of 'view', 'update', 'create', 'delete'.
 * @param $commit
 *   (optional) A git commit to check access for. If nothing is given, access
 *   for all commits is determined.
 * @param $account
 *   The user to check for. Leave it to NULL to check for the global user.
 * @return boolean
 *   Whether access is allowed or not.
 *
 * @see hook_gitcommit_access()
 */
function gitcommit_access($op, $commit = NULL, $account = NULL) {
  if (user_access('administer git commit', $account)) {
    return TRUE;
  }

  $permissions = module_invoke_all('gitcommit_access', $op, $commit, $account);

  if (in_array(FALSE, $permissions, TRUE)) {
    return FALSE;
  }
  elseif (in_array(TRUE, $permissions, TRUE)) {
    return TRUE;
  }

  return FALSE;
}
