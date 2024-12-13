using Plugin.BLE;
using Plugin.BLE.Abstractions.Contracts;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace NeonClockApp
{
    public partial class MainPage : ContentPage
    {
        int count = 0;
        IAdapter _bluetoothAdapter;
        IBluetoothLE _bluetoothBLE;

        private bool isConnected;

        public bool IsConnected
        {
            get => isConnected;
            set
            {
                if (isConnected != value)
                {
                    isConnected = value;
                    OnPropertyChanged();
                    OnPropertyChanged(nameof(ConnectionStatusIcon));
                    OnPropertyChanged(nameof(ConnectionStatusText));
                }
            }
        }

        public string ConnectionStatusIcon => IsConnected ? "\uf293" : "\uf293"; // FontAwesome icons for check and cross
        public string ConnectionStatusText => IsConnected ? "Connected" : "Disconnected";


        protected void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public event PropertyChangedEventHandler PropertyChanged;


        public MainPage()
        {
            InitializeComponent();
            BindingContext = this;
            _bluetoothBLE = CrossBluetoothLE.Current;
            _bluetoothAdapter = CrossBluetoothLE.Current.Adapter;
            _bluetoothAdapter.DeviceDiscovered += OnDeviceDiscovered;
        }

        protected override void OnAppearing()
        {
            base.OnAppearing();
            ScanForDevices(); // Hier die Methode aufrufen
        }

        private async void ScanForDevices()
        {
            if (!_bluetoothBLE.IsOn)
            {
                await DisplayAlert("Bluetooth", "Bitte Bluetooth aktivieren!", "OK");
                return;
            }

            await _bluetoothAdapter.StartScanningForDevicesAsync();
        }

        private void OnDeviceDiscovered(object sender, Plugin.BLE.Abstractions.EventArgs.DeviceEventArgs e)
        {
            var device = e.Device;
            if (device.Name == "NeonClock") // Name deines ESP32-BLE-Geräts
            {
                ConnectToDevice(device);
            }
        }

        private async void ConnectToDevice(IDevice device)
        {
            await _bluetoothAdapter.ConnectToDeviceAsync(device);
            IsConnected = true;
        }

        private void OnCounterClicked(object sender, EventArgs e)
        {
            count++;

            if (count == 1)
                CounterBtn.Text = $"Clicked {count} time";
            else
                CounterBtn.Text = $"Clicked {count} times";

            SemanticScreenReader.Announce(CounterBtn.Text);
        }
    }

}
