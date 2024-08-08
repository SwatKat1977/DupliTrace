# CRON Parser

## Cron expression
A cron expression is a string comprising six fields separated by white space that represents a set of times, normally as a schedule to execute some routine.

| Field        | Required | Allowed values  | Allowed special characters
| ---          | ---      | ---             | ---
| Seconds      | yes      | 0-59            | `*` `,` `-`
| Minutes      | Yes      | 0–59            | `*` `,` `-`
| Hours        | Yes      | 0–23            | `*` `,` `-`
| Day of month | Yes      | 1–31            | `*` `,` `-`
| Month        | Yes      | 1–12 or JAN–DEC | `*` `,` `-`
| Day of week  | Yes      | 0–6 or SUN–SAT  | `*` `,` `-`

The special characters have the following meaning:

| Special character | Meaning    | Description
| ---               | ---        | ---
| `*`               | all values | All values
| `-`               | range      | Range of values
| `,`               | comma      | Additional values
| `/`               | slash      | Increments

Examples: 

| CRON Expression     | Description
| ---                 | ---
| * * * * * *         | Every second
| */10 * * * * *      | Every 10 seconds
| 0 12 9 * * *        | 9:12 AM every day
| 0 30 12 * * MON-FRI | 12:30 PM, Monday to Friday