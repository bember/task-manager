<?php

/**
 * @file
 * gitcommit.bitbucket.inc file.
 */

/**
 * Processor for GitHub commits.
 */
function gitcommit_bitbucket_processor($data) {
  $commits = array();

  if (!empty($data['push']['changes'])) {
    foreach ($data['push']['changes'] as $change_data) {
      if (!empty($change_data['commits'])) {
        foreach ($change_data['commits'] as $commit_data) {
          if (!gitcommit_commit_exists($commit_data['hash'])) {
            $author = $commit_data['author']['raw'];
            preg_match('/\<(.*?)\>/', $author, $matches);

            $values = array(
              'hash' => $commit_data['hash'],
              'message' => $commit_data['message'],
              'time' => strtotime($commit_data['date']),
              'url' => $commit_data['links']['html']['href'],
              'author_mail' => end($matches),
              'repository_name' => $data['repository']['name'],
              'repository_url' => $data['repository']['links']['html']['href'],
            );
            $values['author_name'] = str_replace(' <' . $values['author_mail'] . '>', '', $author);

            $commit = gitcommit_create($values);
            gitcommit_save($commit);

            $commits[] = $commit;
          }
        }
      }
    }
  }

  return $commits;
}
