package clean6265.com.naver.blog.login;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.kongtech.plutocon.sdk.Plutocon;
import com.kongtech.plutocon.sdk.PlutoconManager;
import com.kongtech.plutocon.sdk.connection.PlutoconConnection;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

public class StPlutoconListActivity extends AppCompatActivity implements View.OnClickListener {

    private DatabaseReference mRef;
    private DatabaseReference databaseReference;
    private List<Plutocon> plutoconList;
    private PlutoconAdpater plutoconAdpater;
    private PlutoconManager plutoconManager;
    private Map<String, AnswerHintInformation> map;
    private Button AnswerBtn;
    private TextView CurrentLevelTV;    //학생들의 현재 레벨을 보여주는 TextView입니다.
    private String beaconUUID;
    private String Quest;
    private int Pri;                //데이터베이스에서 가져올 Priority를 저장하는 변수입니다.
    private int CurrentLevel;       //학생들의 현재 우선순위를 저장하는 변수입니다.

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_st_plutocon_list);


        AnswerBtn = (Button)findViewById(R.id.AnswerBtn);
        AnswerBtn.setOnClickListener(this);
        CurrentLevelTV = (TextView)findViewById(R.id.CurrentLevelTV);

        Intent intent1 = getIntent();
         CurrentLevel = intent1.getIntExtra("Priority", 1);//default value는 intent를 통해 키값으로 들어온 값이 없을때, 기본으로 반환하는 값입니다.
        CurrentLevelTV.setText(String.valueOf("My Level: "+CurrentLevel));

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
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {

                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        beaconUUID = plutoconList.get(position).getUuid().toString();
                        if(100<(150+plutoconList.get(position).getRssi())) {
                            if (CurrentLevel > map.get(beaconUUID).Pri) {
                                Intent intent = new Intent(getApplicationContext(), KidsRememberActivity.class);
                                intent.putExtra("beaconUUID", beaconUUID);
                                intent.putExtra("Priority", CurrentLevel);     //다시 돌아오는 상황은 StartActivity로 죽이고 다시 시작하므로 데이터도 전달해야한다.
                                startActivity(intent);
                                finish();
                            } else if (CurrentLevel == map.get(beaconUUID).Pri) {
                                Intent intent = new Intent(getApplicationContext(), KidsGroundActivity.class);
                                intent.putExtra("beaconUUID", beaconUUID);
                                StPlutoconListActivity.this.setResult(RESULT_OK, intent);
                                startActivity(intent);
                                finish();
                            }
                        }
                        else{
                            Toast.makeText(StPlutoconListActivity.this, "너무 멀리있습니다!", Toast.LENGTH_SHORT).show();
                        }

                    }
                }, 0);
            }

        });


        plutoconList = new ArrayList<>();
        plutoconManager = new PlutoconManager(this);
        this.setResult(0, null);

        databaseReference = FirebaseDatabase.getInstance().getReference("BeaconID");

        mRef = FirebaseDatabase.getInstance().getReference("BeaconID");
        map = new HashMap<String, AnswerHintInformation>();

        mRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                if(dataSnapshot != null && dataSnapshot.getValue() != null){
                    for(DataSnapshot postSnapshot : dataSnapshot.getChildren()){
                        map.put(postSnapshot.getKey(), postSnapshot.getValue(AnswerHintInformation.class));
                        //데이터베이스에 있는 모든 uuid들을 가져와서 감지된 리스트들에서 제거하도록 한다.
                    }
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.e("Error! : ", databaseError.getMessage());
            }
        });


    }
    public void onBackPressed(){

    }
    //감지된 플루토콘 리스트들에서 1. 데이터베이스에 없는 uuid의 플루토콘은 없앤다. 2. 우선순위에 맞지 않는 플루토콘을 없앤다.
    private void CmpDatabase(){
        Iterator<Plutocon> it = plutoconList.iterator();
        boolean overlab = false;
        while(it.hasNext()){
            String ituuid = it.next().getUuid().toString();
            for(String uuid : map.keySet()){
                if(ituuid.equals(uuid)){    //데이터베이스에 있는 비콘의 uuid와 겹친다면
                    overlab = true;         //overlab 변수를 true로 만들어 밑에서 지워지지 않도록 한다.
                }
            }
            if(overlab == false){           //하나도 겹치지 않는다면 해당 List에서 삭제한다.
                it.remove();
            }
            overlab = false;
        }
    }
    private void CmpPriority(){
        Iterator<Plutocon> it = plutoconList.iterator();
        while(it.hasNext()){
            String ituuid = it.next().getUuid().toString();
            if(CurrentLevel < map.get(ituuid).Pri){     //현재 학생의 레벨보다 감지된 비콘의 레벨이 더 높을때
                it.remove();
            }
        }
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
                        CmpDatabase();
                        CmpPriority();
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
        if(checkPermission())
            plutoconManager.connectService(new PlutoconManager.OnReadyServiceListener() {
                @Override
                public void onReady() {
                    StPlutoconListActivity.this.startMonitoring();
                }
            });
    }

    @Override
    protected void onPause() {
        super.onPause();
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
                LayoutInflater inflater = LayoutInflater.from(StPlutoconListActivity.this);
                convertView = inflater.inflate(R.layout.item_splutocon, parent, false);
            }

            final Plutocon plutocon = plutoconList.get(position);

            final TextView  tvName = (TextView) convertView.findViewById(R.id.deviceName);
            final TextView  tvLevel = (TextView) convertView.findViewById(R.id.deviceLevel);
            TextView tvRSSI = (TextView) convertView.findViewById(R.id.deviceRSSI);


            tvRSSI.setText("신호 세기: "+(plutocon.getRssi()+150));

            beaconUUID = plutocon.getUuid().toString();
            mRef = databaseReference.child(beaconUUID);


            mRef.child("Quest").addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    Quest = dataSnapshot.getValue(String.class);
                    tvName.setText(Quest);
                }
                @Override
                public void onCancelled(DatabaseError databaseError) {
                }
            });


            mRef.child("Pri").addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    Pri = dataSnapshot.getValue(Integer.class);
                    tvLevel.setText("(Level: "+Pri+")");
                }

                @Override
                public void onCancelled(DatabaseError databaseError) {
                }
            });

/* 잠깐 막아두는것.
            databaseReference.addValueEventListener(new ValueEventListener() {
                @Override
                public void onDataChange(DataSnapshot dataSnapshot) {
                    Quest = dataSnapshot.getValue(String.class);
                    tvLevel.setText(Quest);
                }

                @Override
                public void onCancelled(DatabaseError databaseError) {

                }
            });
*/
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


    @Override
    public void onClick(View view) {

        if(view == AnswerBtn){
            Intent intent = new Intent(getApplicationContext(), OldQuestActivity.class);
            intent.putExtra("Priority", CurrentLevel);
            startActivity(intent);
        }
    }

}