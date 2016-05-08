<?php
/**
 * @file
 * task_manager_task.features.inc
 */

/**
 * Implements hook_node_info().
 */
function task_manager_task_node_info() {
  $items = array(
    'task' => array(
      'name' => t('Task'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
