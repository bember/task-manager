<?php

/**
 * @file
 * gitcommit.github.inc file.
 */

/**
 * Processor for GitHub commits.
 */
function gitcommit_github_processor($data) {
  $commits = array();

  if (!empty($data['commits'])) {
    foreach ($data['commits'] as $commit_data) {
      if (!gitcommit_commit_exists($commit_data['hash'])) {
        $ref = $data['ref'];

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

        foreach (array('added', 'removed', 'modified') as $op) {
          if (!empty($commit_data[$op])) {
            $wrapper->{'gitcommit_files_' . $op}->set($commit_data[$op]);
          }
        }

        $wrapper->save();
        $commits[] = $commit;
      }
    }
  }

  return $commits;
}
