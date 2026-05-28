# Paser

## syntax check : Error
|echo        -> pipe at start
echo |       -> pipe at end with nothing after
echo | | ls  -> two consecutive pipes
echo >       -> redirect with no target
echo > > f   -> redirect followed by redirect