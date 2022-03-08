import argparse

class arguments():
    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--env", help="path to environment file")
        parser.add_argument("--verbosity", nargs="?", type=int, const=1, default=1, help="verbosity level of logging")
        args = parser.parse_args()

        # store path to environment file
        self.env = args.env
        
        # check verbosity arguments
        if args.verbosity >= 0 and args.verbosity <= 4:
            self.verbosity = args.verbosity
