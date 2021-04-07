from exit_class import ExitClass
from typing import IO
from serial import Serial


def loop_serial_write_to_file(serial: Serial, outputfile_IO: IO, exitclass: ExitClass) -> None:
    while not exitclass.exit_now:
        item: bytes = serial.readline()
        item_as_str: str = item.decode("UTF-8", errors="ignore")
        print(f"Read: {item_as_str}")
        outputfile_IO.write(item_as_str)

    print(f"Exit detected - Closing {outputfile_IO.name}")
    outputfile_IO.close()


if __name__ == "__main__":
    exit_class = ExitClass()
    serial_port = "COM3"  # On windows this is COMX, while linux has /dev/ttyS0123
    baud_rate = 115200
    serial_obj = Serial(port=serial_port, baudrate=baud_rate)
    output_file_name = "output.txt"
    output_IO = open(output_file_name, "w+", encoding="UTF-8", errors="ignore")
    loop_serial_write_to_file(serial=serial_obj, outputfile_IO=output_IO, exitclass=exit_class)
