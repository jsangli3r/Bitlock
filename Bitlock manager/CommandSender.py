from Vars import *
from CommandsEnum import *


class CommandSender:

    @staticmethod
    def payload_request(payload_head_start_address):
        if type(payload_head_start_address) is not int:
            raise Exception('payload address must be int')

        command = INCOMING_ASK_PAYLOAD + " " + str(payload_head_start_address)
        Vars.serialcom_instance.send(command)

    @staticmethod
    def payloads_pool_request():
        command = INCOMING_ASK_PAYLOADS_POOL
        Vars.serialcom_instance.send(command)

    @staticmethod
    def security_status_request():
        command = INCOMING_SECURITY_STATUS
        Vars.serialcom_instance.send(command)

    @staticmethod
    def memory_usage():
        command = INCOMING_MEMORY_USAGE
        Vars.serialcom_instance.send(command)

    @staticmethod
    def payload_remove(payload_head_start_address):
        if type(payload_head_start_address) is not int:
            raise Exception('payload address must be int')

        command = INCOMING_PAYLOAD_DELETE
        command += " "
        command += str(payload_head_start_address)

        Vars.serialcom_instance.send(command)

    @staticmethod
    def payload_modify(payload_head_start_address, website, username, password):
        if type(payload_head_start_address) is not int:
            raise Exception('payload address must be int')
        '''if type(website) or type(username) or type(password) is not str:
            raise Exception('payload data must be str')'''

        command = INCOMING_PAYLOAD_CHANGE
        command += " "
        command += str(payload_head_start_address)
        command += " "
        command += website
        command += " "
        command += username
        command += " "
        command += password

        Vars.serialcom_instance.send(command)

    @staticmethod
    def payload_add(website, username, password):
        if type(website) or type(username) or type(password) is not str:
            raise Exception('payload data must be str')

        command = INCOMING_PAYLOAD_ADD
        command += " "
        command += website
        command += " "
        command += username
        command += " "
        command += password

        Vars.serialcom_instance.send(command)
