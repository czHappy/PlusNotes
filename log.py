import logging
from logging.handlers import RotatingFileHandler
import time
# create logger
logger = logging.getLogger('simple_example') 
logger.setLevel(logging.DEBUG)

# create console handler and set level to debug
info_rfh = RotatingFileHandler("logs_info.txt", mode='w', maxBytes=3000, backupCount=1)
info_rfh.setLevel(logging.INFO)
# create formatter
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
# add formatter to ch
info_rfh.setFormatter(formatter)
# add ch to logger
logger.addHandler(info_rfh)

error_rfh = RotatingFileHandler("logs_error.txt", mode='a', maxBytes=3000, backupCount=1)
error_rfh.setLevel(logging.ERROR)
# create formatter
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
# add formatter to ch
error_rfh.setFormatter(formatter)
# add ch to logger
logger.addHandler(error_rfh)


# 'application' code
logger.debug('debug message')
logger.info('info message')
logger.warning('warn message')
logger.error('error message')
logger.critical('critical message')

# for i in range(1, 99999):
    
#     logger.debug('info message: %s', i)
#     logger.info('debug message:%s', i, 'debug message:%s', i)
#     logger.warning('warn message: %s', i)
#     logger.error('error message: %s', i)
#     logger.critical('critical message: %s', i)
#     time.sleep(0.2)
a = 1
b = 2
c = 3
logger.critical("### {} {} {}".format(a, b, c))
# def function_exception():
#     try:
#         a = 1 / 0
#         print(a)
#     except Exception as e:
#         logger.error("catch excetion: %s", e)

# function_exception()