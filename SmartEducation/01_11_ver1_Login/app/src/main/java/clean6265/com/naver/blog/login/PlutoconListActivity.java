package clean6265.com.naver.blog.login;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.LocationManager;
import android.os.Build;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v4.widget.SwipeRefreshLayout;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.kongtech.plutocon.sdk.Plutocon;
import com.kongtech.plutocon.sdk.PlutoconManager;
import com.kongtech.plutocon.sdk.connection.PlutoconConnection;
import com.kongtech.plutocon.sdk.connection.PlutoconEditor;
import com.kongtech.plutocon.sdk.connection.PlutoconOperator;
import com.kongtech.plutocon.sdk.connection.PlutoconReader;
import com.kongtech.plutocon.sdk.repackaged.BluetoothLeUtils;
import com.kongtech.plutocon.sdk.service.PlutoconServive;
import com.kongtech.plutocon.sdk.service.scanner.PlutoconScanner;
import com.kongtech.plutocon.sdk.util.PlutoconUuid;

import java.util.ArrayList;
import java.util.List;

import static java.security.AccessController.getContext;

public class PlutoconListActivity extends AppCompatActivity {
//선생님의 DataUpLoad Activity에서 비콘을 탐색하는 액티비티입니다.
    //아이템을 터치했을때, 비콘의 정보들을 DataUpLoadActivity로 가져옵니다.
    private List<Plutocon> plutoconList;
    private PlutoconAdpater plutoconAdpater;
    private PlutoconManager plutoconManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_plutocon_list);
        final int SELECT_BEACOND_BT =31;
        ProgressBar progress = (ProgressBar) findViewById(R.id.progressBar);
        progress.getIndeterminateDrawable().setColorFilter(
                0xfff00fff,
                android.graphics.PorterDuff.Mode.SRC_IN);

        plutoconAdpater = new PlutoconAdpater();
        ListView listView = (ListView) findViewById(R.id.list);
        listView.setAdapter(plutoconAdpater);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent intent = new Intent();
                intent.putExtra("PLUTOCON", plutoconList.get(position));    //plutocon객체 하나를 intent로 보냅니다.
                PlutoconListActivity.this.setResult(RESULT_OK, intent);
                PlutoconListActivity.this.finish();
            }
        });

        plutoconList = new ArrayList<>();
        plutoconManager = new PlutoconManager(this);
        this.setResult(0, null);
    }

    private void startMonitoring() {
        plutoconManager.startMonitoring(PlutoconManager.MONITORING_FOREGROUND, new PlutoconManager.OnMonitoringPlutoconListener() {
            @Override
            //플루토콘을 발견했을때
            public void onPlutoconDiscovered(Plutocon plutocon, final List<Plutocon> plutocons) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        plutoconList.clear();
                        plutoconList.addAll(plutocons);
                        plutoconAdpater.refresh();
                    }
                });
            }
        });
    }

    @Override
    //프래그먼트가 사용자와 상호작용을 할 수 있게 되었을 때 호출됩니다.
    // 즉, 프래그먼트가 완전히 화면에 표시되어 제 역할을 수행할 수 있게 된 상태입니다.
    protected void onResume() {
        super.onResume();
        Log.e("LogTest", "Call the onResume method");
        if(checkPermission())
            plutoconManager.connectService(new PlutoconManager.OnReadyServiceListener() {
                @Override
                public void onReady() {
                    PlutoconListActivity.this.startMonitoring();
                }
            });
    }


    @Override
    protected void onPause() {
        super.onPause();
        Log.e("LogTest", "Call the onPause method");

        plutoconManager.close();
    }

    private class PlutoconAdpater extends BaseAdapter {

        @Override
        public int getCount() {
            return plutoconList == null ? 0 : plutoconList.size();
        }

        @Override
        public Object getItem(int position) {
            return plutoconList.get(position);
        }

        @Override
        public long getItemId(int position) {
            return 0;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                LayoutInflater inflater = LayoutInflater.from(PlutoconListActivity.this);
                convertView = inflater.inflate(R.layout.item_plutocon, parent, false);
            }

            Plutocon plutocon = plutoconList.get(position);

            TextView tvName = (TextView) convertView.findViewById(R.id.deviceName);
            TextView tvAddress = (TextView) convertView.findViewById(R.id.deviceAddress);
            TextView tvRSSI = (TextView) convertView.findViewById(R.id.deviceRSSI);
            TextView tvInterval = (TextView) convertView.findViewById(R.id.deviceInterval);
            TextView tvUuid = (TextView) convertView.findViewById(R.id.tvUuid);
            TextView tvMajor = (TextView) convertView.findViewById(R.id.tvMajor);
            TextView tvMinor = (TextView) convertView.findViewById(R.id.tvMinor);
            TextView tvLatitude = (TextView) convertView.findViewById(R.id.tvLatitude);
            TextView tvLongitude = (TextView) convertView.findViewById(R.id.tvLongitude);

            tvName.setText(plutocon.getName());
            tvAddress.setText(plutocon.getMacAddress());
            tvRSSI.setText(plutocon.getRssi() + "dBm");
            tvInterval.setText(plutocon.getInterval() + "ms");
            tvUuid.setText(plutocon.getUuid().toString());
            tvMajor.setText(plutocon.getMajor() + "");
            tvMinor.setText(plutocon.getMinor() + "");
            tvLatitude.setText(plutocon.getLatitude() + "");
            tvLongitude.setText(plutocon.getLongitude() + "");

            return convertView;
        }

        private void refresh(){
            notifyDataSetChanged();
        }
    }

    private boolean checkPermission(){
        BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        BluetoothAdapter mBluetoothAdapter = bluetoothManager.getAdapter();

        if((mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled())){
            startActivity(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE));
            return false;
        }


        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            if(checkSelfPermission(android.Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{android.Manifest.permission.ACCESS_COARSE_LOCATION, android.Manifest.permission.ACCESS_FINE_LOCATION}, 1);
                return false;
            }

            LocationManager lm = (LocationManager)getSystemService(Context.LOCATION_SERVICE);
            boolean gps_enabled = false;
            try {
                gps_enabled = lm.isProviderEnabled(LocationManager.GPS_PROVIDER);
            } catch(Exception ex) {}

            if(!gps_enabled){
                startActivity(new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS));
                return false;
            }
        }
        return true;
    }
}