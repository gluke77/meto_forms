CREATE TABLE `meto`.`users` (
  `user` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  `type` INTEGER NOT NULL,
  PRIMARY KEY (`user`)
)
ENGINE = MyISAM
CHARACTER SET cp1251 COLLATE cp1251_general_ci;