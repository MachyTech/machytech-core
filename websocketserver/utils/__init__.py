import argparse
import os
import json
import logging
from dotenv import dotenv_values

ENVFILE_PATH_ERROR = 0
PORTS_AND_HANDLERS_INCOMPATIBLE = 1

class environment():
    def __init__(self, args, logging):
        self.args = args
        self.logging = logging
        # load environment files into config dict
        config = dotenv_values(dotenv_path=str(self.args.env))
        if config:
            self.logging.debug("using {} as environment file".format(str(self.args.env)))
            self.HOST = str(config["MC_HOST"])
            self.PORTS = json.loads(config["MC_PORTS"])
            self.HANDLERS = json.loads(config["MC_HANDLERS"])
            if(len(self.PORTS)!=len(self.HANDLERS)):
                self.error_handler(e=PORTS_AND_HANDLERS_INCOMPATIBLE)
            os.environ
            self.print_environment()
        else:
            self.error_handler(e=ENVFILE_PATH_ERROR)

    def print_environment(self):
        self.logging.debug("Using host ip: {}".format(self.HOST))
        self.logging.debug("Using data ports: {}".format(self.PORTS))

    def error_handler(self, e):
        if e==ENVFILE_PATH_ERROR:
            raise Exception("error reading environment file using default")
        if e==PORTS_AND_HANDLERS_INCOMPATIBLE:
            raise Exception("error reading websocket ports and handlers")

# preferably use our own less complex logging class later
class logger():
    def __init__(self, args):
        # verbosity function
        self.args = args
        self.verbosity = args.verbosity

        self.machyvision_logger = logging.getLogger('MachyVision')
        self.machyvision_logger.setLevel(logging.INFO)

        self.ch = logging.StreamHandler()
        # no verbosity implemented
        self.ch.setLevel(logging.DEBUG)

        self.formatter = logging.Formatter('%(asctime)s - {} - [%(name)s] - [%(levelname)s] - %(message)s'.format(os.uname()[1]), datefmt='%m/%d/%Y %I:%M:%S')
        self.ch.setFormatter(self.formatter)
        self.machyvision_logger.addHandler(self.ch)

    def info(self, string):
        if self.verbosity < 2:
            self.machyvision_logger.info(string)

    def debug(self, string):
        if self.verbosity < 1:
            self.machyvision_logger.info(string)

    def error(self, string):
        if self.verbosity < 4:
            self.machyvision_logger.info(string)

class arguments():
    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--env", help="path tto environment file")
        parser.add_argument("--verbosity", nargs="?", type=int, const=1, default=1, help="verbosity level of logging")
        args = parser.parse_args()

        # store path to environment file
        self.env = args.env

        # check verbosity arguments
        if args.verbosity >= 0 and args.verbosity <=4:
            self.verbosity = args.verbosity
