CREATE TABLE Comment (
    name text,
    targetType text,
    targetName text,
    uid text,
    time int,
    content text
);
CREATE TABLE LikeTable (
    name text,
    targetType text,
    targetName text,
    uid text
);
CREATE TABLE Rating (
    name text,
    targetType text,
    targetName text,
    uid text,
    rating int
);
CREATE TABLE Record (
    id int,
    name text,
    type text,
    item int,
    replay int,
    player text,
    startTime int,
    saveTime int,
    duration float,
    inputOffset float,
    playAreaWidth int,
    playAreaHeight int,
    grade text,
    arcadeScore int,
    accuracyScore int,
    combo int,
    perfect int,
    great int,
    good int,
    miss int,
    totalCount int,
    isPrivate boolean,
    allowRank boolean,
    isRank boolean
);
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
    localization text, 
    isPrivate boolean DEFAULT 0, 
    isRank boolean DEFAULT 1, 
    allowRank boolean DEFAULT 0
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
	localization text,
	creatorId text
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
    uid text,
    session text,
    expire int,
   	body text,
   	signature text
);