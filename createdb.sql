CREATE DATABASE `meto`;

CREATE TABLE `meto`.`users` (
  `user` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  `type` INTEGER NOT NULL,
  PRIMARY KEY (`user`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

CREATE TABLE `meto`.`event_log` (
  `id` INTEGER UNSIGNED NOT NULL DEFAULT NULL AUTO_INCREMENT,
  `code` INTEGER NOT NULL,
  `user_name` VARCHAR(45) NOT NULL,
  `raised` DATETIME NOT NULL,
  `accepted` DATETIME,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

CREATE TABLE  `meto`.`profiles` (
  `dateTime` datetime NOT NULL default '0000-00-00 00:00:00',
  `e2pZone1Sv` double default NULL,
  `e2pZone2Sv` double default NULL,
  `e2pZone3Sv` double default NULL,
  `e2pZone4Sv` double default NULL,
  `e2pTraceSv` double default NULL,
  `e2kZone1Sv` double default NULL,
  `e2kZone2Sv` double default NULL,
  `e2kZone3Sv` double default NULL,
  `e2kTraceSv` double default NULL,
  `e3kZone1Sv` double default NULL,
  `e3kZone2Sv` double default NULL,
  `e3kZone3Sv` double default NULL,
  `e3kTraceSv` double default NULL,
  `e3pZone1Sv` double default NULL,
  `e3pZone2Sv` double default NULL,
  `e3pZone3Sv` double default NULL,
  `e3pZone4Sv` double default NULL,
  `e3pTraceSv` double default NULL,
  `e3mZone1Sv` double default NULL,
  `e3mZone2Sv` double default NULL,
  `e3mZone3Sv` double default NULL,
  `e3mTraceSv` double default NULL,
  `sgZone1Sv` double default NULL,
  `sgZone2Sv` double default NULL,
  `e2pRotate` double default NULL,
  `e2kRotate` double default NULL,
  `e3kRotate` double default NULL,
  `e3pRotate` double default NULL,
  `e3mRotate` double default NULL,
  `sgRotate` double default NULL,
  `tuSpeed` double default NULL,
  `sgDiameter` double default NULL,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY  (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

CREATE TABLE  `meto`.`process_log` (
  `dateTime` datetime NOT NULL default '0000-00-00 00:00:00',
  `e2pZone1Sv` double default NULL,
  `e2pZone1Pv` double default NULL,
  `e2pZone2Sv` double default NULL,
  `e2pZone2Pv` double default NULL,
  `e2pZone3Sv` double default NULL,
  `e2pZone3Pv` double default NULL,
  `e2pZone4Sv` double default NULL,
  `e2pZone4Pv` double default NULL,
  `e2pTraceSv` double default NULL,
  `e2pTracePv` double default NULL,
  `e2kZone1Sv` double default NULL,
  `e2kZone1Pv` double default NULL,
  `e2kZone2Sv` double default NULL,
  `e2kZone2Pv` double default NULL,
  `e2kZone3Sv` double default NULL,
  `e2kZone3Pv` double default NULL,
  `e2kTraceSv` double default NULL,
  `e2kTracePv` double default NULL,
  `e3kZone1Sv` double default NULL,
  `e3kZone1Pv` double default NULL,
  `e3kZone2Sv` double default NULL,
  `e3kZone2Pv` double default NULL,
  `e3kZone3Sv` double default NULL,
  `e3kZone3Pv` double default NULL,
  `e3kTraceSv` double default NULL,
  `e3kTracePv` double default NULL,
  `e3pZone1Sv` double default NULL,
  `e3pZone1Pv` double default NULL,
  `e3pZone2Sv` double default NULL,
  `e3pZone2Pv` double default NULL,
  `e3pZone3Sv` double default NULL,
  `e3pZone3Pv` double default NULL,
  `e3pZone4Sv` double default NULL,
  `e3pZone4Pv` double default NULL,
  `e3pTraceSv` double default NULL,
  `e3pTracePv` double default NULL,
  `e3mZone1Sv` double default NULL,
  `e3mZone1Pv` double default NULL,
  `e3mZone2Sv` double default NULL,
  `e3mZone2Pv` double default NULL,
  `e3mZone3Sv` double default NULL,
  `e3mZone3Pv` double default NULL,
  `e3mTraceSv` double default NULL,
  `e3mTracePv` double default NULL,
  `sgZone1Sv` double default NULL,
  `sgZone1Pv` double default NULL,
  `sgZone2Sv` double default NULL,
  `sgZone2Pv` double default NULL,
  `e2pRotate` double default NULL,
  `e2pCurrent` double default NULL,
  `e2kRotate` double default NULL,
  `e2kCurrent` double default NULL,
  `e3kRotate` double default NULL,
  `e3kCurrent` double default NULL,
  `e3pRotate` double default NULL,
  `e3pCurrent` double default NULL,
  `e3mRotate` double default NULL,
  `e3mCurrent` double default NULL,
  `sgRotate` double default NULL,
  `sgCurrent` double default NULL,
  `sgSpeed` double default NULL,
  `tuRotate` double default NULL,
  `tuCurrent` double default NULL,
  `tuSpeed` double default NULL,
  `e2pPressure` double default NULL,
  `e2kPressure` double default NULL,
  `e3kPressure` double default NULL,
  `e3pPressure` double default NULL,
  `e3mPressure` double default NULL,
  `uzgFreq` double default NULL,
  `uzgCurrent` double default NULL,
  `uzgPower` double default NULL,
  `uzgTemp` double default NULL,
  PRIMARY KEY  (`dateTime`)
) ENGINE=MyISAM DEFAULT CHARSET=cp1251;

INSERT INTO `meto`.`users` (`user`, `password`, `type`)
  VALUES ('meto', 'meto', 2);