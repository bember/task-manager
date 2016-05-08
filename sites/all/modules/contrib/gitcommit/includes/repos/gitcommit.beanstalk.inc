<?php

/**
 * @file
 * gitcommit.beanstalk.inc file.
 */

/**
 * Implements hook_git_commit_webhook_processor.
 */
function gitcommit_beanstalk_gitcommit_webhook_processor($data, $webhook) {
  $commits = array();

  if (!empty($data['commits']) && empty($data['push_is_too_large'])) {
    $values = array();

    foreach ($data['commits'] as $commit_data) {
      $values = array(
        'hash' => $commit_data['id'],
        'message' => $commit_data['message'],
        'time' => strtotime($commit_data['timestamp']),
        'url' => $commit_data['url'],
        'author_mail' => $commit_data['author']['email'],
        'author_name' => $commit_data['author']['name'],
        'repository_name' => $data['repository']['name'],
        'repository_url' => $data['repository']['url'],
      );

      $commit = gitcommit_create($values);
      $wrapper = entity_metadata_wrapper('gitcommit', $commit);

      if (!empty($commit_data['changed_files'])) {
        $files = array();

        foreach ($commit_data['changed_files'] as $tuple) {
          if (empty($files[$tuple[1]])) {
            $files[$tuple[1]] = array();
          }

          $files[$tuple[1]][] = $tuple[0];
        }

        $map = array(
          'add' => 'added',
          'delete' => 'removed',
          'edit' => 'modified',
        );

        foreach ($map as $orig => $dest) {
          if (!empty($files[$orig])) {
            $wrapper->{'gitcommit_files_' . $dest}->set($files[$orig]);
          }
        }
      }

      $wrapper->save();
      $commits[] = $commit;
    }

    return $commits;
  }
}
