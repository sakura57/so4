# Contribution Guidelines

## Getting Started

First, take a look at the design doc (dd.md) which describes the game and its guiding vision. It contains detailed information on all the features that should be present in the base game. When all base features are present, the game is considered ready for the first Alpha release. 

## Pull Requests

I ask a few all-important questions when deciding if a change should be incorporated:

- Does it implement or fix something mentioned directly in the design doc?
- Does the code style fit?
- Is it high-quality code?

If you can answer "yes" to all of the above questions, it's likely the change will be accepted.

## Bug reports

Bug reports should be submitted as an issue on GitHub. To make it easier to verify, fix, and test a reported bug, please use the following format when submitting a bug report:

```
**Version:** Identify in some way the build you were running.

**Description:** A short description of the bug, how it was encountered and its effects.

**Steps to reproduce:** In detail, describe how the bug is caused.

```

An example bug report is as follows:

```
**Version:** latest

**Description:** Attempting to buy equipment that you can't afford causes the game to crash.

**Steps to reproduce:**

- Land on any base.
- Open the Equipment Dealer.
- Buy any piece of equipment that you shouldn't have enough money to be able to purchase.

```