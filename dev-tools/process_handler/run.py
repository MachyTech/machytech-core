from git import Repo
import os
from utils import m_arguments, m_environment, m_logger
import shutil

workspace = os.environ['WORKSPACE']
full_path = str(workspace)+"dev-tools/process_handler/"
args = m_arguments.arguments()
logger = m_logger.logger(args)
env = m_environment.environment(args, logger)

core_repo = Repo(workspace)
assert core_repo
with core_repo.config_writer() as git_config:
    git_config.set_value('user', 'email', env.EMAIL)
    git_config.set_value('user', 'name', env.USER_NAME)
    git_config.set_value('user', 'password', env.TOKEN)

if core_repo.is_dirty(untracked_files=True):
    logger.info("untracked changes in machytech core. Do you want to push repo?")
    
#vision_repo = Repo("https://github.com/MachyTech/machyvision.git")    

# machyvision repository
if not os.path.isdir("machyvision"):
    vision_remote = f"https://{env.USER_NAME}:{env.PASSWORD}@github.com/MachyTech/machyvision.git"
    logger.info("cloning machyvision from: {}".format(vision_remote))
    vision_repo = Repo.clone_from(vision_remote, full_path+"machyvision")
else:
    vision_repo = Repo(full_path + "/machyvision")
    logger.info("machyvision {} branch active".format(vision_repo.active_branch.name))
assert vision_repo


# attempting to install darknet
if not os.path.isdir("machyvision/darknet_sub/"):
    darknet_remote = f"https://github.com/AlexeyAB/darknet.git"
    logger.info("cloning darknet from: {}".format(darknet_remote))
    darknet_repo = Repo.clone_from(darknet_remote, full_path+"machyvision/darknet_sub/")
else:
    darknet_repo = Repo(full_path + "/machyvision/darknet_sub/")
    logger.info("darknet {} branch active".format(darknet_repo.active_branch.name))
assert darknet_repo

# read makefile
with open("machyvision/darknet_sub/Makefile", "r") as reader:
        header = [next(reader) for x in range(9)]
        for i in range(9):
            var = header[i].split("=")
            m_environment.darknet_build_environment.update({var[0]: int(var[1])})
if len((m_environment.darknet_build_environment)) == 9:
    m_environment.compare_and_swap_darknet_build_env(env, 
            m_environment.darknet_build_environment, logger)
    print(m_environment.darknet_build_environment)

with open("machyvision/darknet_sub/Makefile", "r") as reader:
    lines = reader.readlines()

with open("machyvision/darknet_sub/Makefile", "w") as writer:
    i = 0
    for x, y in m_environment.darknet_build_environment.items():
        lines[i]=x+"="+str(y)+"\n"
        i+=1
    writer.writelines(lines)
#with vision_repo.config_writter() as git_config:
#    git_config.set_value('user', 'email', m_environment.EMAIL)
#    git_config.set_value('user', 'name', m_environment.USER_NAME)
#    git_config.set_value('user', 'password', m_environment.PASSWORD)  




