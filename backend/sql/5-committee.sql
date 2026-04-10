CREATE TABLE committee (
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    specialization VARCHAR(255),
    college VARCHAR(255),
    committee ENUM('organizing', 'technical', 'programme','international','industry') NOT NULL,
    profile_picture_object_key VARCHAR(255),
    description TEXT,
    priority INT DEFAULT 0,
);