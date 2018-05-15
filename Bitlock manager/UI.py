from PyQt5.QtWidgets import QWidget, QPushButton,  QMessageBox, QDesktopWidget, QComboBox, QLabel, QLineEdit, QCheckBox
import CommandsEnum
from CommandSender import *
from Payload import Payload
import pyautogui
import pyperclip
import time


class UIWidget(QWidget):

    def __init__(self):
        super().__init__()

        self.connect_btn = None
        self.disconnect_btn = None
        self.remove_btn = None
        self.ask_payloads_btn = None
        self.payloads_combo = None
        self.get_btn = None
        self.website_txtb = None
        self.add_btn = None
        self.modify_btn = None
        self.username_txtb = None
        self.password_txtb = None
        self.password_reveal_check = None
        self.security_status_label = None
        self.memory_usage_label = None

        self.want_to_retrieve_payload = False
        self.payloads_pool_indexes = None

        self.window()

    def connect_button_handler(self):
        try:
            from __init__ import serial_read

            serial_read()

            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)

            msg.setText("We're now connected with success.")
            msg.setInformativeText("Be ready.")
            msg.setWindowTitle("Bitlock")
            msg.setStandardButtons(QMessageBox.Ok)

            msg.exec_()

            self.retrieve_useful_data()
            self.connect_btn.setEnabled(False)

        except Exception as e:
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Critical)

            msg.setText("We can't etablish a connection with the Bitlock external module.")
            msg.setInformativeText(str(e))  # "Please, check your hardware and retry.")
            msg.setWindowTitle("Bitlock")
            msg.setStandardButtons(QMessageBox.Ok)
            msg.exec_()

    @staticmethod
    def retrieve_useful_data():
        # Init, must wait 2 seconds or commands are sent in one time
        time.sleep(2)
        CommandSender.security_status_request()
        time.sleep(2)
        CommandSender.memory_usage()
        time.sleep(2)
        CommandSender.payloads_pool_request()

    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)
        self.move(qr.topLeft())

    def window(self):
        self.connect_btn = QPushButton('Connect arduino', self)
        self.connect_btn.clicked.connect(self.connect_button_handler)
        self.connect_btn.move(30, 10)
        self.connect_btn.resize(180, 50)

        self.ask_payloads_btn = QPushButton('Refresh payloads pool', self)
        self.ask_payloads_btn.clicked.connect(CommandSender.payloads_pool_request)
        self.ask_payloads_btn.move(30, 100)
        self.ask_payloads_btn.resize(180, 50)

        self.payloads_combo = QComboBox(self)
        self.payloads_combo.move((30 + 50 + 150), 10)
        self.payloads_combo.resize(250, 45)
        self.payloads_combo.setEnabled(False)
        self.payloads_combo.activated[str].connect(lambda: self.get_selected_payload(False))

        self.remove_btn = QPushButton('Remove', self)
        self.remove_btn.clicked.connect(self.payload_remove)
        self.remove_btn.move(30, 150)
        self.remove_btn.resize(180, 50)

        self.modify_btn = QPushButton('Modify', self)
        self.modify_btn.clicked.connect(self.payload_modify)
        self.modify_btn.move(30, 200)
        self.modify_btn.resize(180, 50)

        self.add_btn = QPushButton('Add', self)
        self.add_btn.clicked.connect(self.add_payload_button_handler)
        self.add_btn.move(30, 250)
        self.add_btn.resize(180, 50)

        self.disconnect_btn = QPushButton('Disconnect', self)
        self.disconnect_btn.clicked.connect(self.disconnect_button_handler)
        self.disconnect_btn.move(30, 300)
        self.disconnect_btn.resize(180, 50)

        self.get_btn = QPushButton('Output selected payload', self)
        self.get_btn.clicked.connect(lambda: self.get_selected_payload(True))
        self.get_btn.move(30, 350)
        self.get_btn.resize(180, 50)

        self.website_txtb = QLineEdit('', self)
        self.website_txtb.move(230, 100)
        self.website_txtb.resize(250, 25)
        self.website_txtb.setPlaceholderText("Website (or data identifier)")

        self.username_txtb = QLineEdit('', self)
        self.username_txtb.move(230, 150)
        self.username_txtb.resize(250, 25)
        self.username_txtb.setPlaceholderText("Username")

        self.password_txtb = QLineEdit('', self)
        self.password_txtb.setEchoMode(QLineEdit.Password)
        self.password_txtb.move(230, 200)
        self.password_txtb.resize(250, 25)
        self.password_txtb.setPlaceholderText("Password")

        self.password_reveal_check = QCheckBox('Show', self)
        self.password_reveal_check.move(230, 230)
        self.password_reveal_check.stateChanged.connect(self.toggle_password_mask_checkbox_handler)

        self.security_status_label = QLabel('Security status : NC                           ', self)
        self.security_status_label.move(30, 480)

        self.memory_usage_label = QLabel('Used space : NC      ', self)
        self.memory_usage_label.move(300, 480)

        self.resize(500, 500)
        self.setFixedSize(self.size())
        self.center()
        self.setWindowTitle('Bitlock - The \"apparently\" safe password storage')
        self.show()

    def toggle_password_mask_checkbox_handler(self):
        self.password_txtb.setEchoMode(QLineEdit.Normal if self.password_reveal_check.isChecked() else QLineEdit.Password)

    # overwrite from QObject, name must be changed
    @staticmethod
    def disconnect_button_handler():
        Vars.serialcom_instance.close()

    def payloads_pool_combobox_append(self, indexes, websites):
        self.payloads_pool_indexes = indexes

        self.payloads_combo.clear()
        self.payloads_combo.setEnabled(True)
        for website in websites:
            self.payloads_combo.addItem(website)

        if len(self.payloads_pool_indexes) > 0:
            self.payloads_combo.setCurrentIndex(0)

    def get_selected_payload(self, retrieve_payload):
        current_selected_index = self.payloads_combo.currentIndex()
        payload_start_address = self.payloads_pool_indexes[current_selected_index]

        self.want_to_retrieve_payload = retrieve_payload

        CommandSender.payload_request(payload_start_address)

    @staticmethod
    def msg_box(message_type, text, informative_text, buttons):
        msg = QMessageBox()
        msg.setIcon(message_type)

        msg.setText(text)
        msg.setInformativeText(informative_text)
        msg.setWindowTitle("Bitlock")
        msg.setStandardButtons(buttons)

        msg.exec_()

    def security_status_request_refresh_label(self, status):
        status_enum = ['unlocked', 'waiting_both_check', 'waiting_for_fingerprint', 'waiting_for_private_key']

        self.security_status_label.setText('Security status : ' + status_enum[int(status)])

    def add_payload_button_handler(self):
        CommandSender.payload_add(self.website_txtb.text(), self.username_txtb.text(), self.password_txtb.text())

    def add_payload_result_handler(self, result):
        if result:
            self.msg_box(QMessageBox.Information, 'Payload added with success.', '', QMessageBox.Ok)
            self.ask_payloads()  # refresh payload pool

        else:
            self.msg_box(QMessageBox.Critical, 'There was an error, payload not added.', 'Please check your hardware or your data inputs.', QMessageBox.Ok)

    def payload_request_out_handler(self, payload):
        print('10sec left')
        # self.msg_box(QMessageBox.Information, "You'll have 10 seconds after clicking on OK to be in front of the login form, just focus on the email box.", "Then, just wait.", QMessageBox.Ok)

        print(self.want_to_retrieve_payload)
        if self.want_to_retrieve_payload:
            self.want_to_retrieve_payload = False

            time.sleep(10)
            pyperclip.copy(payload.username)
            pyautogui.hotkey('command', 'v')
            pyautogui.press('tab')
            pyperclip.copy(payload.password)
            pyautogui.hotkey('command', 'v')
            pyperclip.copy('')
            pyautogui.press('enter')
        else:
            self.website_txtb.setText(payload.website)
            self.username_txtb.setText(payload.username)
            self.password_txtb.setText(payload.password)

    def memory_usage_changed_label_handler(self, percent):
        text = "Used space : {0}%"
        self.memory_usage_label.setText(text.format(percent))

    def payload_remove(self):
        current_selected_index = self.payloads_combo.currentIndex()
        payload_start_address = self.payloads_pool_indexes[current_selected_index]

        CommandSender.payload_remove(payload_start_address)

    def payload_modify(self):
        current_selected_index = self.payloads_combo.currentIndex()
        payload_start_address = self.payloads_pool_indexes[current_selected_index]

        website = self.website_txtb.text()
        username = self.username_txtb.text()
        password = self.password_txtb.text()

        CommandSender.payload_modify(payload_start_address, website, username, password)

    def payload_modify_out_handler(self, success):
        if success:
            self.retrieve_useful_data()
            print('p edited')
        else:
            print('p not edited')

    def payload_remove_out_handler(self, success):
        if success:
            self.retrieve_useful_data()
            print('p removed')
            #self.msg_box(QMessageBox.Information, "Success", "Payload removed.", QMessageBox.Ok)
        else:
            print('p not removed')
            #self.msg_box(QMessageBox.Critical, "Error", "Something bad occured, please check your hardware", QMessageBox.Ok)