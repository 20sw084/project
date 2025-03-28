# Commande: service Syntax:
service date_service nom : #nom_cocktail ;
## Description:
Records in the log the service of the cocktail list #nom_cocktail to the person `nom` at the time `date_service`.
The `service` command must specify at least one cocktail.
If a cocktail is served `n` times to the same person simultaneously, it is listed #n times.
The `service` command always returns #OK on a single line.
The service orders can be out of chronological order.
## Example Input:
`service 0j_20h05m00s Alice : Eau ;`
`service 0j_20h15m00s Benoit : Jus_Orange ;`
`service 0j_20h40m00s Benoit : Daiquiri_Fraise ;`
`service 0j_20h45m00s Alice : Daiquiri_Framboise ;`
## Expected Output:
`OK`
`OK`
`OK`
`OK`
# Commande: qui Syntax:
qui [date1,date2] nom_cocktail ;
## Description:
Displays the names of all people who were served at least one cocktail from the #nom_cocktail list within the interval [date1,date2].
If no cocktail name is specified, consider all cocktails.
The names are sorted alphabetically (`std::string <` operator).
If no person is found, display `--`.
## Example Input:
qui [0j_00h00m00s,0j_23h59m59s] Eau ;
qui [0j_00h00m00s,0j_23h59m59s] Eau Daiquiri_Fraise ;
qui [0j_00h00m00s,0j_11h59m59s] ;
## Expected Output:
Alice (1)
Alice (1) Benoit (1)
--
# Commande: quoi Syntax:
quoi [date1,date2] nom_personnes ;
## Description:
Displays the compilation of cocktails served to the group of people nom_personnes within the interval [date1,date2].
If no name is specified, analyze all served cocktails.
The cocktails are sorted alphabetically (std::string < operator).
If no results, display --.
## Example Input:
quoi [0j_00h00m00s,0j_23h59m59s] Alice ;
quoi [0j_00h00m00s,0j_23h59m59s] Benoit ;
quoi [0j_00h00m00s,0j_23h59m59s] ;
## Expected Output:
Daiquiri_Framboise (1) Eau (1)
Daiquiri_Fraise (1) Jus_Orange (1)
Daiquiri_Fraise (1) Daiquiri_Framboise (1) Eau (1) Jus_Orange (1)
# Commande: presences Syntax:
presences [date1,date2] ;
## Description:
Displays the names of people present at at least one party between date1 and date2.
The names are sorted alphabetically.
A person is considered present from their first served drink until they finish their last drink.
If no one is found, display --.
## Example Input:
presences [0j_20h30m00s,0j_20h31m00s] ;
presences [0j_20h30m01s,0j_20h31m00s] ;
## Expected Output:
Benoit
--
# Interpretation Rules and Assumptions
Everyone drinks at a constant universal speed: `1 cocktail every 15 minutes`.
Once a person receives a cocktail, they immediately start drinking it.
1. A person drinks only one cocktail at a time. If served multiple cocktails, they drink them one after the other.
2. A party starts at the exact moment the `first drink is served`.
3. A party continues as long as at least one person still has a drink.
4. A party ends when the last person finishes their last drink.
5. A person is considered present at the moment they are served their first drink.
6. A person leaves the party when they finish their last drink.
# Extended Example
## Input (exemple.txt):
service 0j_20h05m00s Alice : Eau ;
service 0j_20h15m00s Benoit : Jus_Orange ;
service 0j_20h40m00s Benoit : Daiquiri_Fraise ;
service 0j_20h45m00s Alice : Daiquiri_Framboise ;
qui [0j_00h00m00s,0j_23h59m59s] Eau ;
qui [0j_00h00m00s,0j_23h59m59s] Eau Daiquiri_Fraise ;
qui [0j_00h00m00s,0j_11h59m59s] ;
quoi [0j_00h00m00s,0j_23h59m59s] Alice ;
quoi [0j_00h00m00s,0j_23h59m59s] Benoit ;
quoi [0j_00h00m00s,0j_23h59m59s] ;
presences [0j_20h30m00s,0j_20h31m00s] ;
presences [0j_20h30m01s,0j_20h31m00s] ;
service 0j_20h15m00s Celine : Daiquiri_Fraise ;
service 0j_20h16m00s Celine : Eau ;
presences [0j_20h30m00s,0j_20h31m00s] ;
presences [0j_21h01m00s,0j_21h02m00s] ;
service 0j_21h05m00s Denis : Margarita ;
service 0j_21h15m00s Celine : Eau Vodka_Canneberges Eau ;
service 0j_22h00m00s Benoit : Jus_Orange ;
service 0j_22h10m00s Denis : Jus_Orange ;
qui [0j_00h00m00s,0j_23h59m59s] Eau ;
quoi [0j_00h00m00s,0j_23h59m59s] Benoit Celine ;
presences [0j_21h01m00s,0j_21h02m00s] ;
presences [0j_21h30m00s,0j_22h05m00s] ;

# Implementation Details
Black intervals → The periods during which people are drinking.
Green dotted lines → The periods during which people are present at the party.
Red intervals (at the top) → The periods when a party is in progress.
After the first four `service` commands, we can track the drink consumption timeline.
After the first six `service` commands, we can determine that Céline was present from 20:15 to 20:45.
As a result, Party (fête) #1 lasted from 20:05 to 21:30.
After executing all ten `service` commands, the final output file is sortie.txt.

# Class and Memory Management
A class or structure named `Interval` can be defined with two public attributes:
start (debut), end (fin).

## Proper use of:
public, protected, and private access modifiers.
The friend keyword where necessary.
# Memory Management
All dynamically allocated memory must be properly freed before the end of main().
A reasonable solution should take only a few seconds to execute (approximately 40 seconds max).
## Command Implementation Order
The presences command is the most complex to implement.
Before handling presences, you must first determine:
The start and end of each party.
When each guest arrives and leaves.
# Recommended Order of Implementation:
1. service
2. qui
3. quoi
4. presences (last)

# Optimization and Execution Time
The implementation must use `binary search trees` (BSTs) for `efficiency`.
The execution time: `40 secs`
Must not exceed twice the execution time of a reasonable solution.
The program must compile with `g++ version 9`.
