to increase(num)
  tell application "System Events"
    repeat num as integer
      delay .05
      key code 144
    end repeat
  end tell
end increase

to decrease(num)
  tell application "System Events"
    repeat num as integer
      delay .05
      key code 145
    end repeat
  end tell
end decrease

on run argv
  if (count of argv > 1) then
    increase(item 2 of argv)
  else
    decrease(item 1 of argv)
  end if
end run