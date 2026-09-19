CREATE TABLE student_committee (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    branch VARCHAR(255),
    profile_picture_object_key VARCHAR(255),
    priority INT DEFAULT 0
);