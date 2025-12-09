#!/usr/bin/env python3
"""
Linux HID Monitor for Windows Precision Trackpad Reports
Reads raw HID reports from /dev/hidrawX and parses them

Usage: sudo python3 ptp_monitor.py /dev/hidrawX
"""
import struct
import sys
import time

def parse_ptp_report(data):
    """Parse our PTP report format"""
    if len(data) < 15:
        return None

    report_id = data[0]

    # Debug: print first few bytes
    if data[0] == 0x01:
        print(f"Input Report (0x01): {' '.join(f'{b:02x}' for b in data[:15])}")

    if report_id == 0x01:  # REPORT_ID_TRACKPAD
        # Contact 0 (bytes 1-5)
        c0_flags = data[1]
        c0_confidence = (c0_flags >> 0) & 1
        c0_tip = (c0_flags >> 1) & 1
        c0_contact_id = (c0_flags >> 2) & 3
        c0_x = struct.unpack('<H', data[2:4])[0]
        c0_y = struct.unpack('<H', data[4:6])[0]

        # Contact 1 (bytes 6-10)
        c1_flags = data[6]
        c1_confidence = (c1_flags >> 0) & 1
        c1_tip = (c1_flags >> 1) & 1
        c1_contact_id = (c1_flags >> 2) & 3
        c1_x = struct.unpack('<H', data[7:9])[0]
        c1_y = struct.unpack('<H', data[9:11])[0]

        # Scan time and contact count
        scan_time = struct.unpack('<H', data[11:13])[0]
        contact_count = data[13]

        # Buttons
        buttons = data[14]
        btn1 = (buttons >> 0) & 1
        btn2 = (buttons >> 1) & 1
        btn3 = (buttons >> 2) & 1

        return {
            'type': 'input',
            'contact_count': contact_count,
            'scan_time': scan_time,
            'c0': {
                'tip': c0_tip,
                'confidence': c0_confidence,
                'id': c0_contact_id,
                'x': c0_x,
                'y': c0_y
            },
            'c1': {
                'tip': c1_tip,
                'confidence': c1_confidence,
                'id': c1_contact_id,
                'x': c1_x,
                'y': c1_y
            },
            'buttons': {'b1': btn1, 'b2': btn2, 'b3': btn3}
        }

    elif report_id in [0x02, 0x03, 0x04, 0x05]:  # Feature reports
        return {
            'type': 'feature',
            'report_id': report_id,
            'data': data[:15]
        }

    return None

def format_report(report, show_all=False):
    """Format report for display"""
    if report['type'] == 'input':
        lines = []
        lines.append(f"Count: {report['contact_count']}, "
                    f"Scan: {report['scan_time']:5d}, "
                    f"Btns: {report['buttons']['b1']}{report['buttons']['b2']}{report['buttons']['b3']}")

        # Only show contacts that are active (or show all if requested)
        if report['c0']['tip'] or show_all:
            lines.append(f"  C0: id={report['c0']['id']}, "
                        f"conf={report['c0']['confidence']}, "
                        f"tip={report['c0']['tip']}, "
                        f"x={report['c0']['x']:4d}, "
                        f"y={report['c0']['y']:4d}")

        if report['c1']['tip'] or show_all:
            lines.append(f"  C1: id={report['c1']['id']}, "
                        f"conf={report['c1']['confidence']}, "
                        f"tip={report['c1']['tip']}, "
                        f"x={report['c1']['x']:4d}, "
                        f"y={report['c1']['y']:4d}")

        return '\n'.join(lines)

    elif report['type'] == 'feature':
        hex_str = ' '.join(f'{b:02x}' for b in report['data'])
        return f"Feature Report ID: 0x{report['report_id']:02x}, Data: {hex_str}"

def main():
    if len(sys.argv) < 2:
        print("Usage: sudo python3 ptp_monitor.py /dev/hidrawX [--all]")
        print("\nOptions:")
        print("  --all    Show all contacts, even when not touching")
        print("\nExample:")
        print("  sudo python3 ptp_monitor.py /dev/hidraw4")
        sys.exit(1)

    device_path = sys.argv[1]
    show_all = '--all' in sys.argv

    print(f"Opening {device_path}...")
    print("Touch the trackpad to see reports (Ctrl+C to exit)")
    print("=" * 70)

    try:
        with open(device_path, 'rb') as f:
            report_count = 0
            last_report_time = time.time()

            while True:
                data = f.read(64)
                if not data:
                    time.sleep(0.001)
                    continue

                report = parse_ptp_report(data)
                if report:
                    report_count += 1
                    current_time = time.time()
                    delta_ms = (current_time - last_report_time) * 1000
                    last_report_time = current_time

                    # Only print if there's activity or we're in verbose mode
                    if report['type'] == 'input':
                        if show_all or report['contact_count'] > 0 or report_count % 100 == 0:
                            print(f"\n[{report_count:5d}] Δ{delta_ms:5.1f}ms")
                            print(format_report(report, show_all))
                    else:
                        # Always show feature reports
                        print(f"\n[{report_count:5d}] Δ{delta_ms:5.1f}ms")
                        print(format_report(report, show_all))

    except KeyboardInterrupt:
        print("\n" + "=" * 70)
        print(f"Captured {report_count} reports")
    except FileNotFoundError:
        print(f"Error: Device {device_path} not found")
        print("\nTry: ls -la /sys/class/hidraw/")
        sys.exit(1)
    except PermissionError:
        print(f"Error: Permission denied. Run with sudo:")
        print(f"  sudo python3 {sys.argv[0]} {device_path}")
        sys.exit(1)

if __name__ == "__main__":
    main()
