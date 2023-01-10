CREATE TABLE Level (id int, name text, version int, rating int, title text, artists text, author text, engine int, skin int, background int, effect int, particle int, cover text, bgm text, data text, preview text);
CREATE TABLE Skin (id int, name text, version int, title text, subtitle text, author text, thumbnail text, data text, texture text);
CREATE TABLE Background (id int, name text, version int, title text, subtitle text, author text, thumbnail text, data text, image text, configuration text);
CREATE TABLE Effect (id int, name text, version int, title text, subtitle text, author text, thumbnail text, data text, audio text);
CREATE TABLE Particle (id int, name text, version int, title text, subtitle text, author text, thumbnail text, data text, texture text);
CREATE TABLE Engine (id int, name text, version int, title text, subtitle text, author text, skin int, background int, effect int, particle int, thumbnail text, data text, configuration text, rom text);