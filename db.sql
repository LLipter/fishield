CREATE TABLE user(
    username varchar(100) PRIMARY KEY,
    password varchar(100) NOT NULL,
    privilege int DEFAULT 0
)default charset=utf8;

INSERT INTO user('LLipter', '123456', 10);
INSERT INTO user('test', '000000', 0);