import logging
import os

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