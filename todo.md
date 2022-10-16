# Important
- Remove code from API module to API_Headers to use as client lib to save API module code only in server and improve security
- Not forgot about create embed module to spoof client PC as requested attachment to proof completion of job by worker

# Normal
- Create Request class to pass to Handler
- Add to API handler ban user
- Add updater module to client
- add more concrete cmd errors
- add iiServer error messages presets
- restructure Tokens table for trace User token usage
- add default icon images for Job, User and user grade
- for all API handlers create parser
- create API handler inputs reader
- add server ddos protection
- add server max tries to identify/register with timer

# Unimportant
- add new module config reader
- create BururyConfig to write all global shit
- split code of different classes from PagesManager to other files
- rename enums from iiNPack to concrete
- Think about pack nodejs to client bandle to upload pre run scripts in nodejs code
- Create manager register procedure
- Add workers liderboard
- Edit CreateTable handler input to table name
