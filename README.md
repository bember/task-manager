#Task Manager

Demo project for [Git Commit Webhook](https://www.drupal.org/project/gitcommit) module.

It's using 7.x-dev version.

## Testing integration
1\. Enable contrib modules:
```sh
drush en -y gitcommit gitcommit_reference
```

2\. Enable custom modules:
```sh
drush en -y gitcommit_comments task_manager_task
```

3\. Configure Git Commit Webhook by accessing */admin/config/gitcommit/settings*. You are able to set:
- Webhook status (enabled, disabled)
- Expected repo (GitHub, Bitbucket, Beanstalk)
- Security token
- Regex to reference Drupal entities from a given commit message

4\. Choose a repository (or create a sandbox repo), find Webhook settings and create a new webhook, setting *http://YOUR_WEBSITE/git-commit-webhook* as Payload URL. Here are the documentation of each repo supported by Git Commit Webhook:
- [GitHub](https://developer.github.com/webhooks/creating/)
- [Bitbucket](https://confluence.atlassian.com/bitbucket/manage-webhooks-735643732.html)
- [Beanstalk](http://support.beanstalkapp.com/article/931-classic-webhooks-integration)

How to test it in a local environment? I suggest using [Ultrahook](http://www.ultrahook.com/)

5\. Create a new Task node: *Content > Add content > Task*, or access */node/add/task*

6\. Make changes in your repo and commit them, making a reference to the task ID. Here is an example, referencing a task node whose nid is 1:
```sh
git commit -m "#1: Testing webhook integration."
```

That's it! you should see a new comment in your task node.
