Api commands now responces with json objects and contains all input data that can be modified. Also can contain importan uniq sub objects(command objects) which impacts on client app.
Now we have that command objects:

# Notifications
Send notify mail to client
"notify": {
                "title": "notify mail title",
                "message": "mail box text",
                "level": "normal"
}
