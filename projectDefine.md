main screen
    build encounter button
    Start encounter button
        new encounter button
        existing encounter button
    browse creatures button
        search bar
        creature1...creature2...
            remove creature option on custom creatures
            edit creature option on custom creatures
        add creature button
    browse players button
        search bar
        add player button
        player1
        player2
        ...
            remove player
            edit player

encounter db screen
clicking encounter header passes name shown by the header into the callback
callback looks up the encounter in the db based on the name
lookup then looks up the players and creatures based on the encounter name and stores them in buildlistmembers
then execute a version of BuildEncounterChain that doesn't allow for text editing