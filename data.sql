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