CREATE DATABASE IF NOT EXISTS ps_gris;

USE ps_gris;

CREATE TABLE [IF NOT EXISTS] pessoas
(
	`pessoa_id` INT AUTOINCREMENT PRIMARY KEY ,
	`nome` VARCHAR(100) NOT NULL ,
	`data_de_nascimento` DATE ,
	`dre` CHAR(9) NOT NULL ,
	`curso` CHAR(50) NOT NULL ,
) ENGINE=INNODB;

CREATE TABLE [IF NOT EXISTS] alunos
(
	`aluno_id` INT AUTOINCREMENT ,
	PRIMARY KEY (aluno_id),
	FOREIGN KEY (pessoa_id),
		REFERENCES pessoas (pessoa_id)
) ENGINE=INNODB;

CREATE TABLE [IF NOT EXISTS] palestrantes
(
	`palestrante_id` INT AUTOINCREMENT ,
	PRIMARY KEY (palestrante_id),
	FOREIGN KEY (pessoa_id),
		REFERENCES pessoas (pessoa_id)
) ENGINE=INNODB;

CREATE TABLE [IF NOT EXISTS] tags
(
	`tag_id` INT AUTOINCREMENT ,
	PRIMARY KEY (tag_id),
	`descricao` TEXT ,
	`data_entrega` DATE NOT NULL ,
	FOREIGN KEY (palestrante_id),
		REFERENCES palestrantes (palestrante_id)
) ENGINE=INNODB;
