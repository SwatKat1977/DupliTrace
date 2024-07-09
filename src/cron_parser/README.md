# CRON Parser

## Cron expression
A cron expression is a string comprising five fields separated by white space that represents a set of times, normally as a schedule to execute some routine.

Field        | Required | Allowed values  | Allowed special characters
---          | ---      | ---             | ---
Minutes      | Yes      | 0–59            | * , -
Hours        | Yes      | 0–23            | * , -
Day of month | Yes      | 1–31            | * , -
Month        | Yes      | 1–12 or JAN–DEC | * , -
Day of week  | Yes      | 0–6 or SUN–SAT  | * , -

The special characters have the following meaning:

Special character | Meaning    | Description
---               | ---        | ---
*                 | all values | All values
-                 | range      | Range of values
,                 | comma      | Additional values
/                 | slash      | Increments
