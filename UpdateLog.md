# Upcoming `0.7.3` Engine Preview Mode (1/2)

Specs: <https://github.com/Sonolus/wiki-engine-specs/tree/develop/docs>

For skin, new standard `#GRID_` sprites are available and skins should implement them. Engines can use these sprites in preview mode to draw lines (although engines are discouraged to use them for everything, eg don't use them for lanes but use actual lane sprites)
Sprites: <https://github.com/Sonolus/sonolus-core/blob/develop/src/common/core/skin/skin-sprite-name.ts#L101-L107>

Simple overview of preview mode:
- Preview mode simply draws everything onto a canvas like a static image, and player can scroll around.
- Typically you draw the level as a 2D image like various chart viewers.
- Implement archetypes of interest. Eg if you want to draw notes, implement the note archetype that has the same name as in level data.
- `preprocess` callback should have very light workload, most of the work should be done in `render` callback.
- Typically in `preprocess` callback you would setup UI, and canvas (scrolling direction and size).
- For engines that don't have length available in level so canvas size can't be calculated directly, you would have variables storing the latest beat/time of notes, and each note entity updates those variables in `preprocess` callback.
- `render` callback simply does the drawing.

# Upcoming `0.7.3` Engine Preview Mode (2/2)

For Sonolus.js users:
- There shouldn't be any breaking changes and your existing code should still work.
- You can use Sekai engine as a reference for preview mode implementation: <https://github.com/NonSpicyBurrito/sonolus-pjsekai-engine/tree/develop/preview>
- You can now omit the file names in CLI arguments and configs, and it will look for `sonolus-cli.config.*` and `index.*` automatically. You can clean up your paths: <https://github.com/NonSpicyBurrito/sonolus-pjsekai-engine/commit/b3bc24b0970c24997fd1459a69eef5a757cfab8e>

Sonolus.js guide: <https://github.com/Sonolus/wiki-sonolus.js-guide/tree/develop/docs>

Sonolus.js project templates are updated, you can bootstrap using the `0.7.3` branches.

# Upcoming `0.7.4` Engine Changes (1/2)

## Configuration
- New progress UI configuration, used in preview and watch mode.

## Play Mode
- Remove autoplay, replaced by watch mode.

## Preview Mode
- Add progress UI.

## New Watch Mode
Allows player to watch a level with advanced features like pause/resume/time skip.

Specs: <https://github.com/Sonolus/wiki-engine-specs/tree/develop/docs>
Runtime definitions: https://discord.com/channels/696599620259807242/949074528884129824/1175665405361147924
Sonolus.js Guide: <https://github.com/Sonolus/wiki-sonolus.js-guide/tree/develop/docs>
Sonolus.js Guide Code: <https://github.com/Sonolus/wiki-sonolus.js-guide-code>

Overview:
- Watch mode can be thought of as a scripted video.
- When player uses progress bar to time skip, the `skip` value in Runtime Update block will be set to `1`.
- Entity spawning/despwaning is entirely managed by Sonolus runtime.
- Watch mode has a global `updateSpawn` callback, where you need to return a value indicating the current "time" (it can be any timeline)
- Archetype has `spawnTime` and `despawnTime` callback where you need to return values indicating when said entity is alive.
- Any entity where the current time is within spawn/despawn time range, will be automatically spawned; and automatically despawned when outside of range.
- `Spawn` function can only be called during `preprocess`. Entities spawned by `Spawn` will also be managed the same way as regular entities.
- Entities with input need to set `time` in Entity Input block.
- SFX must be scheduled during `preprocess` using the scheduled variants. Non scheduled functions are not supported.
- Particles will be cleared when time skips.

# Upcoming `0.7.4` Engine Changes (2/2)

Patterns and gotchas in watch mode:
- Progress UI is recommended to be full width at bottom of the screen.
- `updateSpawn` typically returns the current scaled time (or time if the engine does not use time scale)
- Note's `spawnTime` typically returns the minimum visual time, and `despawnTime` typically returns the maximum visual time.
- For entities that should always be alive, you can simply return large numbers (eg `spawnTime` returns `-999999`, `despawnTime` returns `999999`)
- For entities that have despawn time smaller than spawn time, they will never be spawned.
- An entity can be spawned and despawned multiple times due to time skip.
- Because of multiple spawns, `initialize` callback may be called multiple times. If you have logic that is only supposed to execute once, do something like this:
```ts
initialize() {
    if (hasInitialized) return
    hasInitialized = true

    // One time initialization logic here
}
```
- An entity can be despawned because it leaves the time range, or it could be because player used time skip. Logics in your `terminate` callback that are only meant to be executed when leaving the time range (such as note playing particle effects), should check the `skip` value in Runtime Update block:
```ts
terminate() {
    if (skip) return

    // Spawn particle effect
}
```
- For entities that deal with persistent particle effects (such as hold connectors), keep in mind that particle effects are cleared on every time skip, so you must spawn them again.
- For Sonolus.js users, packages are available under `beta` tag.
- For Sonolus.js users, you can use Sekai engine as reference implementation: <https://github.com/NonSpicyBurrito/sonolus-pjsekai-engine/tree/develop>

# Upcoming `0.8.0` Engine Changes (1/3)

No specs are available yet, you can type type definition in: <https://github.com/Sonolus/sonolus-core/tree/sonolus-0.8.0>

## Engine
- Version updates to `12`.

## Level Data
- Entity `ref` is now renamed to `name` (however entity data `ref` is still `ref`, no change)

## Engine Play/Watch/Preview Data
- Archetype `data` is now renamed to `imports`.

## Engine Play Data
- Archetype now has `exports`, for exporting data to be saved in a replay and used in watch mode.

## Engine Watch Data
- Now has `buckets`, for result screen when watching replay.

## Play Mode
- New `ExportValue(index, value)` function to export value for the specific index of `exports`.

## Watch Mode
- Now has `inputOffset` and `replay` value in Runtime Environment block.
- New Level Bucket block (similar to play mode)
- Two special archetype import name `#JUDGMENT` and `#ACCURACY`.

## Overview of Replay
- At play mode result screen, player can now create a replay. Sonolus will store all necessary information in replay configuration and replay data resources.
- A replay can be watched using watch mode of the engine.
- In play mode, engine can use `exports` to export values needed in order for watch mode to watch the replay.
- In watch mode, engine can use `imports` to import not only values from Level Data, but also values exported by play mode, as well as the special `#JUDGMENT` and `#ACCURACY` values.
- Using these imported values, watch mode needs to reconstruct what the replay gameplay was like, so players can watch and study it.

# Upcoming `0.8.0` Engine Changes (2/3)

## Replay Implementation Guide
- Determine what data is necessary for watch mode to reconstruct a replay's gameplay. For most engines, judgment and accuracy are enough and they are automatically available for importing. For some engines you might need more data, such as activation/deactivation times of sliders in Sekai.
- In play mode code, implement exporting those data.
- In watch mode code, implement importing those data and changing gameplay based on them.
- Note despawn time in watch mode: because accuracy value is supposed to be `hitTime - targetTime`, so despawn time can be simply shifted by adding its accuracy.
- Note hit effects in watch mode: change it depending on its judgment. This includes particle effects, SFX (Perfect/Great/Good sounds), lane effects, etc.
- Sim line despawn time in watch mode: change it depending on connecting notes despawn times.
- Buckets in watch mode: implementation should be the same as play mode.

## Optimizations
- Use the `replay` value in Runtime Environment block to determine if watch mode is currently watching replay or not, and skip unnecessary code.
- When exporting values in play mode, exporting `0` will be optimized away by Sonolus in the final replay data file to not take up any extra space, so encode your data in a way that can best make use of this to reduce replay data file size.

# Upcoming `0.8.0` Engine Changes (3/3)

## For Sonolus.js Users
- You can use Sekai engine for reference: <https://github.com/NonSpicyBurrito/sonolus-pjsekai-engine/tree/sonolus-0.8.0>
- `defineData` is now renamed to `defineImport`.
- `defineExport` available for play mode, and returns a function you can call to export:
```ts
class Note extends Archetype {
    export = defineExport({
        foo: { name: 'ExportNameForFoo', type: Number },
        bar: { name: 'ExportNameForBar', type: Boolean },
        baz: { name: 'ExportNameForBaz', type: DataType<MyEnum> },
    })

    updateParallel() {
        this.export('foo', 42)
        this.export('bar', true)
        this.export('baz', MyEnum.MagicValue)
    }
}
```
- You can use `replay.isReplay` in watch mode to check if it's currently watching replay.

# Upcoming `0.8.0` Custom Server Changes for Multiplayer (1/5)

Specs are not yet available, use [`sonolus-0.8.0` branch in `sonolus-core` repo](<https://github.com/Sonolus/sonolus-core/tree/sonolus-0.8.0>) as reference for type definitions.

## `GET /sonolus/info`
- New `hasMultiplayer` indicates if server has multiplayer.
```diff
type ServerInfo = {
+   hasMultiplayer: boolean
}
```

## `GET /sonolus/rooms/info`
Identical to other item types.

## `GET /sonolus/rooms/list`
Identical to other item types.

## `POST /sonolus/rooms/create`
- When client attempts to create a room, a request with body of `CreateRoomRequest` will be sent.
- If server decides room creation is allowed, should respond with a body of `CreateRoomResponse`.
- Once player has entered the room, exiting the app to share room link for other players to join might cause disconnect, so `name` contains the name of room to be created and allows room creator to share the room link before the room has finished being created.
- Server should either create the room immediately and allow everyone to join but reserving a spot for the room creator, or reserve the room until room creator has finished creating.
- When client has finished creating, it joins the room with create options as query with header `Sonolus-Room-Key` containing the value of `key`.
- Server can use `Sonolus-Room-Key` to verify the client joining is the room creator, and create/reconfigure the room based on query.
```ts
type CreateRoomRequest = {}

type CreateRoomResponse = {
    name: string
    key: string
    creates: ServerOptionsSection[]
}
```

# Upcoming `0.8.0` Custom Server Changes for Multiplayer (2/5)

## `POST /sonolus/rooms/{name}`
- When client attempts to join a room, a request with body of `JoinRoomRequest` will be sent with header `Sonolus-Signature` containing signature of the body signed by Sonolus.
- Server should verify that: the signature matches body; `type` is of value `authenticateMultiplayer`; `address` is server's address; `room` is room's name; `time` is recent.
- If server decides joining is successful, should respond with a body of `JoinRoomResponse`.
- `url` contains the URL to WebSocket multiplayer server (`wss://...` or `ws://...`), `type` contains the room type (currently only `round`).
- Connect request to the multiplayer server will have `Sonolus-Room-Session` header with value of `session`, server can use it to relay information to the multiplayer server.
```ts
type JoinRoomRequest = {
    type: 'authenticateMultiplayer'
    address: string
    room: string
    time: number
    userProfile: UserProfile
}

type JoinRoomResponse = {
    url: string
    type: 'round'
    session: string
}
```

## Rooms
```ts
type RoomItem = {
    name: string
    title: string
    subtitle: string
    master: string
    tags: Tag[]
    cover?: SRL
    bgm?: SRL
    preview?: SRL
}
```

## Search
Server search is now renamed to `ServerOptionsSection` and is used in many places, eg room creation options.
```diff
- type Search = {
+ type ServerOptionsSection = {
    type: string
    title: Text | (string & {})
    icon?: Icon
    options: ServerOption[]
}
```

## SRL
Type is no longer necessary.
```diff
- type SRL<T extends ResourceType> = {
-   type: T
+ type SRL = {
    hash: string
    url: string
}
```

## Ease
- Ease is now `camelCase`.
- This affects UI in Engine Configuration and Particle Data.
- Particle version bumped to `3`.

## Engine Play Mode
- A new `multiplayer` value in Runtime Environment block.

# Upcoming `0.8.0` Custom Server Changes for Multiplayer (3/5)

## Architecture
- Multiplayer is a WebSocket server.
- Messages from server to clients are called events, messages from clients to server are called commands. Type definitions can be found in `sonolus-core` repo.
- Server owns the room state (room status, current selected level, players in the room, etc)
- Client upon joining the room, first receives an `UpdateEvent` and makes a local copy of the room state.
- When server changes the room state, server should send the corresponding event to clients.
- Clients upon receiving events, mutate their local copies of the room state accordingly to keep in sync with server's room state.
- Clients when performing actions, send the corresponding commands to server.
- If an command is allowed, server mutates its room state and send event to clients.
- To improve UX in high latency situations, client has optimistic update.
- This means that clients may send invalid commands to server, and server should be tolerant to invalid commands.
- Clients however are not tolerant to invalid events, so server should always make sure events are valid.

## Initialization and Suspension
- When client first connects to the server, it's in an uninitialized state, and will ignore all received messages until an `UpdateEvent`.
- When client starts gameplay, it enters a suspended state, and will ignore all received messages.
- When client exits gameplay, it sends a `FinishGameplayCommand` and reverts back to uninitialized state, awaiting an `UpdateEvent`.

## Optimizations
Events are designed in a way to accommodate common state mutations such that multiplayer server can scale with large amount of players.
For example, when a player has finished gameplay and server needs to update the score of said player, server sending the entire scoreboard (n players' scores) to all clients (n players) would result in an O(n^2) update.
Instead, server can send a specialized event achieving O(n).

# Upcoming `0.8.0` Custom Server Changes for Multiplayer (4/5)

## Round Room Gameplay
- Room has three phases: selecting, preparing, and playing.
- During selecting: room lead can select which level to play, change standard level options and auto exit, and manage suggestions; other players can add levels to suggestions. Once room lead clicks confirm, room moves to preparing phase.
- During preparing: every player can change configuration, and ready/skip. Once room master clicks start, room moves to playing phase.
- During playing: readied players will start playing the level; skipped players remain in the room waiting for results and can participate in other activities such as chatting and suggesting for next round's level. Once room master clicks finish, room moves back to selecting phase.

## Room Master
- Room master can: give room master to another player; change who is room lead; kick players; starts and finishes rounds; edit room options.
- Server can set room master to `room` so no player is room master.

## Room Lead
- Room lead can: select level; change standard level options; change auto exit; manage suggestions.
- Server can set room lead to `room` so no player is room lead.

## Suggestions
- Suggestions is a collaborative editing level list that can be used for multiple purposes.
- The most basic usage is for other players to suggest levels to room lead.
- However server can use it as a playlist, as a way for everyone to pick a level and randomize which one to be played, as a way for a competitive match to do pick and bans, etc.

## Chat
- Self explanatory.
- Server can use chat to implement more complex features such as bot commands.

## Scoreboard
- Scoreboard is completely customizable by the server, and can be used to implement any type of scoring.
- Different scoreboard sections can be used to group players together, and can be used to represent teams.
- Player scores are strings, so you can use not just numbers but also texts like "winner" etc.

# Upcoming `0.8.0` Custom Server Changes for Multiplayer (5/5)

## Implementation Strategy
- Start with a representation of room state.
- When a client connects, implement sending connected client `UpdateEvent`; implement sending other clients `AddUserEvent`.
- When a client disconnects, implement sending other clients `RemoveUserEvent`.
- When receiving a command from client, implement handling the command by mutating room state and sending events to all clients.

## Implementation Notes
- `RoomUser`: `authentication` is the body of `JoinRoomRequest` base64 encoded, `signature` is the `Sonolus-Signature` header of `JoinRoomRequest`.
- `AddChatMessageCommand`: `nonce` should be echoed back to the sender's `AddChatMessageEvent`, but omitted from other clients.
- `state` in `StartRoundEvent`: will be echoed back by clients in `StartGameplayCommand` and `FinishGameplayCommand`, server should use it to check if the round has already ended.
- `seed` in `StartRoundEvent`: should be a number between `0` and `1`, will be used to populate PRNG so every client will randomize the same.
- `UpdateLevelEvent`: upon receiving, client will reset level options to empty.
- `UpdateStatusEvent`: upon receiving, if changing status to selecting, client will set all user statuses to waiting; if changing status to playing, client will reset results to empty, and set all skipped user's statuses to waiting.
- `UpdateStatusEvent`: upon receiving, if changing status to playing, server should send `StartRoundEvent` to readied clients.
- `FinishGameplayCommand`: upon receiving, client will go back to uninitialized state and wait for an `UpdateEvent`.
- `AddUserEvent`: upon receiving, client will set added user's status to waiting.
- `RemoveUserEvent`: upon receiving, client will set master to room if removed user was master; will set lead to room if removed user was lead; will remove removed user's result; will remove removed user's status; will remove removed user from scoreboard sections.

# Upcoming `0.8.2` Custom Server Changes (1/2)

Specs and types are not yet available.

## `GET /sonolus/{type}/{name}`
- New `hasCommunity` indicates if item has community section.
```diff
type ItemDetails<T> = {
    item: T
    description: string
+   hasCommunity: boolean
    sections: ItemSection<T>[]
}
```

## `GET /sonolus/{type}/{name}/community`
- Actions are presented as buttons, clicking will bring up a form for player to fill out and submit.
- If there are top comments, more button is available to expand into fully paginated comments list; otherwise "no comments" is display and cannot expand.
```ts
type ItemCommunity = {
    actions: ServerForm[]
    topComments: ItemCommunityComment[]
}
```

## `GET /sonolus/{type}/{name}/community/comments/list`
- Paginated comment list.
```ts
type ItemCommunityCommentList = {
    pageCount: number
    comments: ItemCommunityComment[]
}
```

## `POST /sonolus/{type}/{name}/community`
- Requested when player submits an action.
- Server can use response to instruct Sonolus to update community section.
```ts
type SubmitItemCommunityActionRequest = {
    values: string
}

type SubmitItemCommunityActionResponse = {
    shouldUpdateCommunity?: boolean
    shouldNavigateCommentsToPage?: number
}
```

# Upcoming `0.8.2` Custom Server Changes (2/2)

## New `limit` in text option
```diff
type ServerTextOption = {
    query: string
    name: Text | (string & {})
    type: 'text'
    placeholder: Text | (string & {})
+   limit?: number
}
```

## New text area option
```ts
type ServerTextAreaOption = {
    query: string
    name: Text | (string & {})
    type: 'textArea'
    placeholder: Text | (string & {})
    limit?: number
}
```

## `ServerOptionsSection` renamed to `ServerForm`
```diff
- type ServerOptionsSection = {
+ type ServerForm = {
    type: string
    title: Text | (string & {})
    icon?: Icon
    options: ServerOption[]
}
```

## `ItemCommunityComment`
```ts
type ItemCommunityComment = {
    author: string
    time: number
    content: string
    actions: ServerForm[]
}
```

# Upcoming `0.8.3` Custom Server Changes (1/4)

Specs and types are not yet available. This update includes breaking changes.

## `GET /sonolus/info`
- Now server specifies a list of buttons to be displayed on home page.
```diff
type ServerInfo = {
    title: string
    description?: string
-   hasAuthentication: boolean
-   hasMultiplayer: boolean
+   buttons: ServerInfoButton[]
    banner?: SRL
}
```
```ts
type ServerInfoButton = {
    type:
        | 'authentication'
        | 'multiplayer'
        | 'post'
        | 'playlist'
        | 'level'
        | 'replay'
        | 'skin'
        | 'background'
        | 'effect'
        | 'particle'
        | 'engine'
}
```

## `GET /sonolus/{type}/info`
- Now has optional `creates` property.
- If present, a create button will be visible, and player can click it to bring up the forms to fill out to create an item.
- Creating process will lead to `/create` and `/upload` endpoints below.
```diff
type ItemInfo<T> = {
+   creates?: ServerForm[]
    searches?: ServerForm[]
    sections: ItemSection<T>[]
    banner?: SRL
}
```

## `POST /sonolus/{type}/create`
- After filling out the create form, a request with form values will be posted.
- If success, server should respond with `name` of the created item.
- If additionally files need to be uploaded, the response should also include an upload `key`, and a list of `hashes` of files to be uploaded.
- Hashes should be obtained from the form values.
```ts
type CreateItemRequest = {
    values: string
}

type CreateItemResponse = {
    name: string
    key: string
    hashes: string[]
}
```

## `POST /sonolus/{type}/upload`
- Client will post to this endpoint if `/create` needs files to be uploaded.
- `Sonolus-Upload-Key` will contain the value of `key`.
- Body will be a multipart form with files specified by `hashes`.
```ts
type UploadItemResponse = {}
```

# Upcoming `0.8.3` Custom Server Changes (2/4)

## `GET /sonolus/{type}/{name}`
- New optional leaderboard feature.
- A list of leaderboards can be specified with `leaderboards` property.
- Player can select to view each leaderboard.
- All item types can have leaderboards, not limited to levels. Server can for example have a post for a seasonal competition, and use post's leaderboard for competition's player rankings.
```diff
type ItemDetails<T> = {
    item: T
    description: string
    hasCommunity: boolean
+   leaderboards: ItemLeaderboard[]
    sections: ItemSection<T>[]
}
```
```ts
type ItemLeaderboard = {
    name: string
    title: Text | (string & {})
}
```

## `GET /sonolus/{type}/{itemName}/leaderboards/{leaderboardName}`
- Details of a leaderboard.
- It should contain the top records; if signed in, should also show player's placement.
- Player can further expand to see all records of the leaderboard.
```ts
type ItemLeaderboardDetails = {
    topRecords: ItemLeaderboardRecord[]
}

type ItemLeaderboardRecord = {
    name: string
    rank: Text | (string & {})
    player: string
    value: Text | (string & {})
}
```

## `GET /sonolus/{type}/{itemName}/leaderboards/{leaderboardName}/records/list`
- Paginated list of the records of a leaderboard.
```ts
type ItemLeaderboardRecordList = {
    pageCount: number
    records: ItemLeaderboardRecord[]
}
```

## `GET /sonolus/{type}/{itemName}/leaderboards/{leaderboardName}/records/{recordName}`
- Details of a record, contains replays.
```ts
type ItemLeaderboardRecordDetails = {
    replays: ReplayItem[]
}
```

# Upcoming `0.8.3` Custom Server Changes (3/4)

## `POST /sonolus/rooms/create`
- Removed `creates`.
```diff
type CreateRoomResponse = {
    name: string
    key: string
-   creates: ServerForm[]
}
```

## `GET /sonolus/{type}/{name}/community/info`
- Moved endpoint.
```diff
- GET /sonolus/{type}/{name}/community
+ GET /sonolus/{type}/{name}/community/info
```

## `POST /sonolus/{type}/{name}/community/submit`
- Moved endpoint.
- Now only community actions are submitted here.
```diff
- POST /sonolus/{type}/{name}/community
+ POST /sonolus/{type}/{name}/community/submit
```

## `ItemCommunityComment`
- Now has `name` property.
- Will be used for submitting a comment's action.
```diff
type ItemCommunityComment = {
+   name: string
    author: string
    time: number
    content: string
    actions: ServerForm[]
}
```

## `POST /sonolus/{type}/{itemName}/community/comments/{commentName}/submit`
- Now comments' actions are submitted here.
- Same as `POST /sonolus/{type}/{name}/community/submit`.

# Upcoming `0.8.3` Custom Server Changes (4/4)

## `ServerOption`
- Now has optional `description` property.
- Now has optional `required` property. If `true`, will require player to modify the value before submitting.
```diff
+ description?: string
+ required?: boolean
```

## `ServerServerItemOption`
- New option type which allows player to select an item from the current server.
- Value will be `name` of the selected item.
```ts
type ServerServerItemOption = {
    query: string
    name: Text | (string & {})
    description?: string
    required?: boolean
    type: 'serverItem'
    itemType: ItemType
}
```

## `ServerCollectionItemOption`
- New option type which allows player to select an item from collections.
- Value will be JSON of the selected item.
```ts
type ServerCollectionItemOption = {
    query: string
    name: Text | (string & {})
    description?: string
    required?: boolean
    type: 'collectionItem'
    itemType: ItemType
}
```

## `ServerFileOption`
- New option type which allows player to select a file.
- Value will be hash of the selected file.
```ts
type ServerFileOption = {
    query: string
    name: Text | (string & {})
    description?: string
    required?: boolean
    type: 'file'
}
```

# Upcoming `0.8.4` Custom Server Changes (1/4)

Specs and types are not yet available. This update includes breaking changes.

## JSON
- Stricter JSON validation and detailed validation errors.

## Server Configuration
- New server configuration.
- To offer configuration, server should include a button of type `configuration`.
- Configuration has general options (localization and auto login), as well as server specified options.
- Server specified option values will be included in every request's query just like localization.
- For servers that don't have options, configuration button is optional, but it might still be offered for localization and auto login.
```diff
type ServerInfo = {
    title: string
    description?: string
    buttons: ServerInfoButton[]
+   configuration: ServerConfiguration
    banner?: Srl
}

type ServerInfoButton = {
    type:
        | 'authentication'
        // ...
+       | 'configuration'
}
```
```ts
type ServerConfiguration = {
    options: ServerOption[]
}
```

## Item Details Actions
- Server can add actions to item details, which will appear as buttons alongside existing buttons like Save/Share/Update/etc.
- This can be used to implement likes/dislikes, rating an item, editing, etc.
- Players can click the buttons to fill out forms and submit actions to `POST /sonolus/{type}/{itemName}/submit`.
```diff
type ServerItemDetails<T> = {
    item: T
    description?: string
+   actions: ServerForm[]
    hasCommunity: boolean
    leaderboards: ServerItemLeaderboard[]
    sections: ServerItemSection<T>[]
}
```

## `POST /sonolus/{type}/{itemName}/submit` and `POST /sonolus/{type}/{itemName}/upload`
- Endpoint for submitting item details actions and uploading.
```ts
type ServerSubmitItemActionRequest = {
    values: string
}

type ServerSubmitItemActionResponse = {
    shouldUpdate?: boolean
    shouldRemove?: boolean
    key: string
    hashes: string[]
}
```

# Upcoming `0.8.4` Custom Server Changes (2/4)

## Item Community Actions and Comment Actions
- Now can upload.
```diff
type ServerSubmitItemCommunityActionResponse = {
    shouldUpdateCommunity?: boolean
    shouldNavigateCommentsToPage?: number
+   key: string
+   hashes: string[]
}
```

> ## Item Leaderboard
> - Now can have description.
> ```diff
> type ServerItemLeaderboard = {
>     name: string
>     title: Text | (string & {})
> +   description?: string
> }
> ```

## Server Item Section
- Now can display items of any type, specified by `itemType` (eg `'level'`).
- Now has `searchValues`, if present, a more button will be available and clicking will bring player to lists with the search values as queries.
- Now has `search`, if present, a search button will be available and clicking will bring up search form with search values pre filled.
```diff
type ServerItemSection<T> = {
    title: Text | (string & {})
    icon?: Icon | (string & {})
+   itemType: ItemType
    items: T[]
+   search?: ServerForm
+   searchValues?: string
}
```

## Server Form
- Now can have description.
- Now can required player to confirm before submitting.
```diff
type ServerForm = {
    type: string
    title: Text | (string & {})
    icon?: Icon | (string & {})
+   description?: string
+   requireConfirmation: boolean
    options: ServerOption[]
}
```

## SRL
- Now `hash` and `url` are optional.
```diff
type Srl = {
-   hash: string
+   hash?: string | null
-   url: string
+   url?: string | null
}
```

## SIL
- Sonolus item locator, used in various places such as forms and multiplayer.
```ts
type Sil = {
    address: string
    name: string
}
```

# Upcoming `0.8.4` Custom Server Changes (3/4)

## Server Options
- `required` now required.
```diff
type ServerOption = {
    query: string
    name: Text | (string & {})
    description?: string
-   required?: boolean
+   required: boolean
}
```

## Server Text Option
- New `def` for default value.
- `limit` now required, use `0` for unlimited.
- New `shortcuts` for a list of shortcut buttons player can use to quickly insert texts.
```diff
type ServerTextOption = {
    // ...
    type: 'text'
+   def: string
    placeholder: Text | (string & {})
-   limit?: number
+   limit: number
+   shortcuts: string[]
}
```

## Server Text Area Option
- New `def` for default value.
- `limit` now required, use `0` for unlimited.
- New `shortcuts` for a list of shortcut buttons player can use to quickly insert texts.
```diff
type ServerTextAreaOption = {
    // ...
    type: 'textArea'
+   def: string
    placeholder: Text | (string & {})
-   limit?: number
+   limit: number
+   shortcuts: string[]
}
```

## Server Toggle Option
- `def` is now `boolean`.
```diff
type ServerToggleOption = {
    // ...
    type: 'toggle'
-   def: 0 | 1
+   def: boolean
}
```

## Server Multi Option
- `defs` now renamed to `def`.
```diff
type ServerMultiOption = {
    // ...
    type: 'multi'
-   defs: boolean[]
+   def: boolean[]
    values: (Text | (string & {}))[]
}
```

## Server Server Item Option
- New `def` for default value.
- New `allowOtherServers` for controlling if selecting from another server is allowed.
- Serialized value now uses SIL.
```diff
type ServerServerItemOption = {
    // ...
    type: 'serverItem'
    itemType: ItemType
+   def: Sil | null
+   allowOtherServers: boolean
}
```

## Server Server Items Option
- New option for selecting multiple server items.
```ts
type ServerServerItemsOption = {
    // ...
    type: 'serverItems'
    itemType: ItemType
    def: Sil[]
    allowOtherServers: boolean
    limit: number
}
```

# Upcoming `0.8.4` Custom Server Changes (4/4)

## Multiplayer
- Now uses SIL instead of level locator.
```diff
type AddSuggestionCommand = {
    type: 'addSuggestion'
-   level: LevelLocator
+   level: Sil
}

type UpdateLevelCommand = {
    type: 'updateLevel'
-   level: LevelLocator
+   level: Sil
}

type UpdateEvent = {
    type: 'update'
    // ...
-   level?: LevelLocator
+   level: Sil | null
    // ...
}

type UpdateLevelEvent = {
    type: 'updateLevel'
-   level?: LevelLocator
+   level: Sil | null
}

type Suggestion = {
    userId: ServiceUserId
-   level: LevelLocator
+   level: Sil
}
```

## Multiplayer
- Now uses `null` for room as master/lead rather than `'room'`.
```diff
type UpdateEvent = {
    type: 'update'
    // ...
-   master: 'room' | ServiceUserId
+   master: ServiceUserId | null
-   lead: 'room' | ServiceUserId
+   lead: ServiceUserId | null
    // ...
}

type UpdateLeadEvent = {
    type: 'updateLead'
-   lead: 'room' | ServiceUserId
+   lead: ServiceUserId | null
}

type UpdateMasterEvent = {
    type: 'updateMaster'
-   master: 'room' | ServiceUserId
+   master: ServiceUserId | null
}

type TextChatMessage = {
-   userId: 'room' | ServiceUserId
+   userId: ServiceUserId | null
    type: 'text'
    value: string
}
```