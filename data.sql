CREATE TABLE Level (
    id int, 
    name text, 
    version int, 
    rating int, 
    title text, 
    artists text, 
    author text, 
    engine int, 
    skin int, 
    background int, 
    effect int, 
    particle int, 
    cover text, 
    bgm text, 
    data text, 
    preview text
);
CREATE TABLE Skin (
    id int, 
    name text, 
    version int, 
    title text, 
    subtitle text, 
    author text, 
    thumbnail text, 
    data text, 
    texture text
);
CREATE TABLE Background (
    id int, 
    name text,
    version int, 
    title text, 
    subtitle text, 
    author text, 
    thumbnail text, 
    data text, 
    image text, 
    configuration text
);
CREATE TABLE Effect (
    id int,
    name text, 
    version int, 
    title text, 
    subtitle text, 
    author text, 
    thumbnail text, 
    data text, 
    audio text
);
CREATE TABLE Particle (
    id int, 
    name text, 
    version int, 
    title text, 
    subtitle text, 
    author text, 
    thumbnail text, 
    data text, 
    texture text
);
CREATE TABLE Engine (
    id int, 
    name text, 
    version int, 
    title text, 
    subtitle text, 
    author text, 
    skin int, 
    background int, 
    effect int, 
    particle int, 
    thumbnail text, 
    data text, 
    configuration text, 
    rom text
);
CREATE TABLE UserProfile (
    id text,
    handle text,
    name text,
    avatarForegroundColor text,
    avatarBackgroundColor text,
    aboutMe text,
    socialLinks text,
    favorites text
);
CREATE TABLE UserSession (
    id text,
    aes_key text,
    aes_iv text,
    time int
);
CREATE TABLE LoginRequest (
    code text,
    session text,
    time int,
    userId text,
    userAgent text,
    ip text
);

/* Sonolus Server v1.4.4 */
ALTER TABLE Level ADD COLUMN description text;
ALTER TABLE Skin ADD COLUMN description text;
ALTER TABLE Background ADD COLUMN description text;
ALTER TABLE Effect ADD COLUMN description text;
ALTER TABLE Particle ADD COLUMN description text;
ALTER TABLE Engine ADD COLUMN description text;
ALTER TABLE Level ADD COLUMN localization text DEFAULT 'default';
ALTER TABLE Skin ADD COLUMN localization text DEFAULT 'default';
ALTER TABLE Background ADD COLUMN localization text DEFAULT 'default';
ALTER TABLE Effect ADD COLUMN localization text DEFAULT 'default';
ALTER TABLE Particle ADD COLUMN localization text DEFAULT 'default';
ALTER TABLE Engine ADD COLUMN localization text DEFAULT 'default';

/* Sonolus v0.7.2 */
ALTER TABLE Engine ADD COLUMN tutorialData text;

/* Sonolus v0.7.3 */
ALTER TABLE Engine ADD COLUMN previewData text;

/* Sonolus v0.7.4 */
ALTER TABLE Engine ADD COLUMN watchData text;

/* Sonolus v0.8.0 */
ALTER TABLE Level ADD COLUMN tags text;
ALTER TABLE Skin ADD COLUMN tags text;
ALTER TABLE Background ADD COLUMN tags text;
ALTER TABLE Effect ADD COLUMN tags text;
ALTER TABLE Particle ADD COLUMN tags text;
ALTER TABLE Engine ADD COLUMN tags text;
CREATE TABLE Replay (
    id int, 
    name text, 
    version int,
    title text, 
    subtitle text, 
    author text, 
    level int,
    data text, 
    configuration text,
    tags text,
    description text,
    localization text DEFAULT 'default'
);
CREATE TABLE Post (
    id int,
    name text,
    version int,
    title text,
    time int,
    author text,
    thumbnail text,
    tags text,
    description text,
    localization text DEFAULT 'default'
);
CREATE TABLE Playlist (
    id int,
    name text,
    version int,
    title text,
    subtitle text,
    author text,
    levels text,
    thumbnail text,
    tags text,
    description text,
    localization text DEFAULT 'default'
);
DROP TABLE UserSession;
DROP TABLE LoginRequest;
CREATE TABLE UserSession (
    uid text,
    session text,
    expire int,
   	body text,
   	signature text
);
CREATE TABLE Room (
	id int,
	name text,
	version int,
	title text,
	subtitle text,
	master text,
	cover text,
	bgm text,
	preview text,
	tags text,
	localization text DEFAULT "default",
	creatorId text
);