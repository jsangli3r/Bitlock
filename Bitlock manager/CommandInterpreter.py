#"1 Tout est ok"
#"password_ask hash_key github.com"
#"password_block_ask <hash_key> <website>"
#packet ou commande ?

import CommandsEnum
from Vars import *
from Payload import Payload
from PyQt5.QtWidgets import QMessageBox

""" Route specific command to specific associated function, received command """


def command_router(raw_command):
    print(raw_command)  # debug

    if raw_command.find("\"") != -1:
        first_quote_index = raw_command.index("\"")
        second_quote_index = raw_command.index("\"", first_quote_index)
        temp_2quote_param_str = ""
        parameters = raw_command.split(" ")
        command = parameters[0]
        parameters.pop(0)

        i = 0
        for x in parameters:
            if first_quote_index <= i <= second_quote_index:
                temp_2quote_param_str += x
                parameters.pop(i)
            i += 1

        temp_2quote_param_str.replace("\"", "")
        parameters.insert(first_quote_index, temp_2quote_param_str)
    else:
        parameters = raw_command.split(" ")
        command = parameters[0]
        parameters.pop(0)

    if command == CommandsEnum.OUTGOING_MESSAGE:
        message_out(parameters)

    elif command == CommandsEnum.OUTGOING_ASK_PAYLOADS_POOL:
        payloads_pool_request_out(parameters)

    elif command == CommandsEnum.OUTGOING_ASK_PAYLOAD:
        payload_request_out(parameters)

    elif command == CommandsEnum.OUTGOING_SECURITY_STATUS:
        security_status_request_out(parameters)

    elif command == CommandsEnum.OUTGOING_PAYLOAD_ADD:
        payload_add_out(parameters)

    elif command == CommandsEnum.OUTGOING_MEMORY_USAGE:
        memory_usage_out(parameters)

    elif command == CommandsEnum.OUTGOING_PAYLOAD_DELETE:
        payload_delete_out(parameters)


def message_out(parameters):
    if len(parameters) != 1:
        return

    Vars.user_interface.msg_box(QMessageBox.Information, parameters[0], "", QMessageBox.Ok)


def payloads_pool_request_in():
    send_command(CommandsEnum.INCOMING_ASK_PAYLOADS_POOL)


def payload_add_out(parameters):
    if len(parameters) != 1:
        return

    add_result = bool(parameters[0])

    Vars.user_interface.add_payload_result_handler(add_result)


def payload_remove_out(parameters):
    return 0


# <w> <u> <p>
def payload_request_out(parameters):
    if len(parameters) != 3:
        return

    website = parameters[0]
    username = parameters[1]
    password = parameters[2]
    payload = Payload(website, username, password)  # just to make data as clear as possible we put in a object

    Vars.user_interface.payload_request_out_handler(payload)


def payloads_pool_request_out(parameters):
    indexes = []
    websites = []

    for param in parameters:
        splited_param = param.split(";")
        print("param = " + param)
        print("[0] = " + splited_param[0])
        print("[1] = " + splited_param[1])

        indexes.append(int(splited_param[0])) #convert to int
        websites.append(splited_param[1])

    Vars.user_interface.payloads_pool_combobox_append(indexes, websites) #call the Bitlock ui function to interprete data by passing tuple


def security_status_request_out(parameters):
    if len(parameters) != 1:
        return

    status = parameters[0]
    Vars.user_interface.security_status_request_refresh_label(status)


def memory_usage_out(parameters):
    if len(parameters) != 1:
        return

    percent = parameters[0]
    Vars.user_interface.memory_usage_changed_label_handler(percent)


def payload_delete_out(parameters):
    if len(parameters) != 1:
        return

    remove_result = bool(parameters[0])

    Vars.user_interface.payload_remove_out_handler(remove_result)


def payload_modify_out(parameters):
    if len(parameters) != 1:
        return

    modify_result = bool(parameters[0])
    Vars.user_interface.payload_modify_out_handler(modify_result)

def send_command(command):
    if type(command) is not str:
        raise Exception('Error')

    Vars.serialcom_instance.send(command)
