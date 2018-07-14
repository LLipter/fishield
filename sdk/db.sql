CREATE TABLE user(
    username VARCHAR(100) PRIMARY KEY,
    password VARCHAR(100) NOT NULL,
    privilege int DEFAULT 0,
    create_time DATETIME DEFAULT NOW()
)default charset=utf8;

INSERT INTO user(username, password, privilege) VALUES('LLipter', '123456', 10);
INSERT INTO user(username, password) VALUES('test', '000000');


CREATE TABLE task(
    task_id INT PRIMARY KEY,
    localbasepath VARCHAR(512),
    remotebasepath VARCHAR(512),
    filename VARCHAR(256),
    total_packet_no INT,
    task_status INT,
    received_packet_no INT,
    sent_packet_no INT,
    last_packet_time BIGINT
)default charset=utf8;