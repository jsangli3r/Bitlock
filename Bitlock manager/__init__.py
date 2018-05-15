"""
PPL Manager - alpha
COMBY D. SANGLIER J.
17-18
"""
import threading
import sys
from PyQt5.QtWidgets import QApplication
from Vars import *
from SerialCom import SerialCom
from UI import UIWidget

# GLOBAL VAR
# user_interface = None
# serialcom_instance = None

# Full duplex communication thread
def serial_read():
    try:
        # global serialcom_instance
        Vars.serialcom_instance = SerialCom()
        t1 = threading.Thread(target=Vars.serialcom_instance.wait_read, args=())
        t1.start()
    except Exception as e:
        print(str(e))


def main():
    app = QApplication(sys.argv)
    Vars.user_interface = UIWidget()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
