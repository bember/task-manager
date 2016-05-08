<?php

/**
 * @file
 * Provides a base class for Git Commit entity.
 */

/**
 * The class used for Git commit entities.
 */
class GitCommit extends Entity {

  /**
   * The commit internal id.
   *
   * @var integer
   */
  public $cid;

  /**
   * The hash ID.
   *
   * @var string
   */
  public $hash;

  /**
   * The commit message.
   *
   * @var string
   */
  public $message;

  /**
   * The Unix timestamp when the commit happened.
   *
   * @var integer
   */
  public $time;


  /**
   * The commit URL.
   *
   * @var string
   */
  public $url;

  /**
   * E-mail of commit author.
   *
   * @var string
   */
  public $author_mail;

  /**
   * Name of commit author.
   *
   * @var string
   */
  public $author_name;

  /**
   * The repository name.
   *
   * @var string
   */
  public $repository_name;

  /**
   * The repository URL.
   *
   * @var string
   */
  public $repository_url;

  /**
   * Overrides Entity::defaultLabel().
   */
  public function defaultLabel() {
    return $this->message;
  }
}
