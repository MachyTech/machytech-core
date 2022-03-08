from utils import m_logger, m_arguments, m_enums
import os
import json
from dotenv import dotenv_values

class environment():
    def __init__(self, args, logging):
        self.args = args
        self.logging = logging
        self.notify_darknet_build_changes = 0
        # load environment files into config dict
        shared_config = dotenv_values(dotenv_path=str(self.args.env)+".shared")
        if shared_config:
            self.logging.debug("using {} as environment file".format(str(self.args.env)+".shared"))   
            self.COMPILE_WITH_GPU = int(shared_config["USE_DARKNET_GPU"])
            if self.COMPILE_WITH_GPU==1:
                self.COMPILE_WITH_CUDNN = 1
    
            self.COMPILE_WITH_CUDNN_HALF= int(shared_config["USE_CUDNN_HALF"])
            self.COMPILE_WITH_LIBSO = 1
            self.COMPILE_WITH_OPENCV = 1
            self.COMPILE_WITH_OPENMP = int(shared_config["USE_OPENMP"])
            self.COMPILE_WITH_AVX = int(shared_config["USE_AVX"])

            # overriding os environment variables
            os.environ
            self.print_shared_environment()
        secret_config = dotenv_values(dotenv_path=str(self.args.env)+".secret")
        if secret_config:
            self.logging.debug("using {} as environment file".format(str(self.args.env)+".secret "))
            self.EMAIL = str(secret_config["GIT_EMAIL"])
            self.USER_NAME = str(secret_config["GIT_USER_NAME"])
            self.TOKEN = str(secret_config["GIT_TOKEN"])
            # overriding os environment variables
            self.print_secret_environment()
        else:
            self.error_handler(e=m_enums.ENVFILE_PATH_ERROR)
        
    def print_shared_environment(self):
        self.logging.debug("Using darknet gpu: {}".format(self.COMPILE_WITH_GPU))
        self.logging.debug("Using darknet cudnn half: {}".format(self.COMPILE_WITH_CUDNN_HALF))
        self.logging.debug("Using darknet openmp: {}".format(self.COMPILE_WITH_OPENMP))
        self.logging.debug("Using darknet avx: {}".format(self.COMPILE_WITH_AVX))

    def print_secret_environment(self):
        self.logging.debug("Using git email: {}".format(self.EMAIL))
        self.logging.debug("Using git user name: {}".format(self.USER_NAME))
        self.logging.debug("Using git password: {}".format(self.TOKEN))
    def error_handler(self, e):
        if e==m_enums.ENVFILE_PATH_ERROR:
            raise Exception("error reading environment file using default")

def compare_and_swap_darknet_build_env(env, dict, logger):
    if dict["GPU"]!=env.COMPILE_WITH_GPU:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment GPU from {} to {}".format(dict["GPU"], env.COMPILE_WITH_GPU))
        dict.update({"GPU": env.COMPILE_WITH_GPU})
    if dict["CUDNN"]!=env.COMPILE_WITH_CUDNN_HALF:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment CUDNN from {} to {}".format(dict["CUDNN"], env.COMPILE_WITH_CUDNN))
        dict.update({"CUDNN": env.COMPILE_WITH_CUDNN})
    if dict["CUDNN_HALF"]!=env.COMPILE_WITH_CUDNN_HALF:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment CUDNN_HALF from {} to {}".format(dict["CUDNN_HALF"], env.COMPILE_WITH_CUDNN_HALF))
        dict.update({"CUDNN_HALF": env.COMPILE_WITH_CUDNN_HALF}) 
    if dict["OPENCV"]!=env.COMPILE_WITH_OPENCV:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment OPENCV from {} to {}".format(dict["OPENCV"], env.COMPILE_WITH_OPENCV))
        dict.update({"OPENCV": env.COMPILE_WITH_OPENCV})
    if dict["AVX"]!=env.COMPILE_WITH_AVX:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment AVX from {} to {}".format(dict["AVX"], env.COMPILE_WITH_AVX))        
        dict.update({"AVX": env.COMPILE_WITH_AVX})
    if dict["OPENMP"]!=env.COMPILE_WITH_OPENMP:
        env.notify_darknet_build_changes+=1
        logger.debug("change build environment MP from {} to {}".format(dict["OPENMP"], env.COMPILE_WITH_OPENMP))
        dict.update({"OPENMP": env.COMPILE_WITH_OPENMP})
darknet_build_environment = {}