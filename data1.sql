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
    configuration text, 
    description text, 
    localization text, 
    tags text
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
    audio text, 
    description text, 
    localization text, 
    tags text
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
    rom text, 
    description text, 
    localization text, 
    tutorialData text, 
    previewData text, 
    watchData text, 
    tags text
);
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
    preview text, 
    description text, 
    localization text, 
    tags text
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
    texture text, 
    description text, 
    localization text, 
    tags text
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
    localization text
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
    localization text
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
    texture text, 
    description text, 
    localization text, 
    tags text
);