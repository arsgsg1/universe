package clean6265.com.naver.blog.login;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import java.util.ArrayList;
import java.util.Collections;

public class OldQuestActivity extends AppCompatActivity {

    private int CurrentLevel;
    private ListView mListView = null;
    private ListViewAdapter mAdapter = null;
    private AlertDialog mDialog = null, mSelectDialog = null, mModifyQuestDialog = null;

    private ArrayList<ListData> arrayList = new ArrayList<ListData>();
    private Uri filepath;
    private String FileType, beaconUUID;

    private static final int PICK_IMAGE_REQUEST =234;

    private FirebaseAuth firebaseAuth;
    private DatabaseReference mRef;
    private StorageReference storageReference;
    Thread readyThread =null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_old_quest);
        storageReference = FirebaseStorage.getInstance().getReference();



        Intent intent = getIntent();
        CurrentLevel = intent.getIntExtra("Priority", 1);
        //arrayList = (ArrayList) intent.getSerializableExtra("arrayList");
        //직렬화를 통해 전달된 인텐트이므로 받을 땐 역직렬화를 통해 받습니다.
        //역 직렬화는 getSerializableExtra를 통해 받고, 캐스팅을 합니다.
        //캐스팅을 하는 이유는 byte단위의 스트림을 통해 온 메모리형태를 객체가 요구하는
        //메모리의 형태로 다시 잡아주어야 하기 때문입니다.


        mListView = (ListView) findViewById(R.id.qList);

        mAdapter = new ListViewAdapter(this);
        mListView.setAdapter(mAdapter);




        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View v, int position, long id){
                ListData mData = (ListData)arrayList.get(position);
                beaconUUID = mData.mDate;   //uuid획득

                Intent intent1 = new Intent(getApplicationContext(), KidsRememberActivity.class);
                intent1.putExtra("beaconUUID", beaconUUID);
                intent1.putExtra("Priority", CurrentLevel);     //다시 돌아오는 상황은 StartActivity로 죽이고 다시 시작하므로 데이터도 전달해야한다.
                startActivity(intent1);
                finish();
            }
        });

        mRef = FirebaseDatabase.getInstance().getReference("BeaconID"); //DataBase의 BeaconID의 하위항목의 전부를 참조하도록 합니다.

        mRef.addValueEventListener(new ValueEventListener() {   //firebase의 DataBase에서 정보들을 모두 가져오도록 합니다.
            //또한 이후에 값이 바뀌더라도 ValueEventListener에서 그것을 감지하여 onDataChange메소드를 호출합니다.
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                arrayList.clear();
                for(DataSnapshot postSnapshot : dataSnapshot.getChildren()){    //getChildren()메소드는 BeaconID의 하위 항목 전체를 iterator형식으로 한번씩 반환합니다.
                    //즉, BeaconID의 하위 항목 전체를 처음부터 끝까지 한번씩 반환하겠다는 의미입니다.
                    AnswerHintInformation temp = postSnapshot.getValue(AnswerHintInformation.class);
                    ListData data = new ListData();
                    data.mPri = temp.Pri;
                    data.mDate = postSnapshot.getKey(); //getKey()메소드는 해당 항목의 key값을 반환합니다.
                    data.mTitle = temp.Quest;

                    if(data.mPri<CurrentLevel)arrayList.add(data);


                }
                mAdapter.dataChange();

            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                Log.e("DataBaseError!: ", "Please confirm the database");
            }



        });



    }
    public void onBackPressed(){
        Intent intent = new Intent(this, StPlutoconListActivity.class);
        intent.putExtra("Priority", CurrentLevel);
        startActivity(intent);
        finish();
        super.onBackPressed();
    }




    private class ViewHolder{

        public ImageView mIcon;
        public TextView mTitle;
        public TextView mUUID;


    }

    private class ListViewAdapter extends BaseAdapter {
        private Context mContext = null;
        //private ArrayList<ListData> mListData = new ArrayList<ListData>();

        public ListViewAdapter(Context mContext) {
            super();
            this.mContext = mContext;

        }

        @Override
        public int getCount() {

            return arrayList.size();
        }

        @Override
        public Object getItem(int position) {

            return arrayList.get(position);
        }

        @Override
        public long getItemId(int position) {

            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            ViewHolder holder;
            if (convertView == null) {
                holder = new ViewHolder();

                LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(R.layout.listview_item, null);

                holder.mIcon = (ImageView) convertView.findViewById(R.id.mImage);
                holder.mTitle = (TextView) convertView.findViewById(R.id.mTitle);
                holder.mUUID = (TextView) convertView.findViewById(R.id.mUUID);


                ListData data = (ListData)arrayList.get(position);
                holder.mTitle.setText(data.mTitle);
                holder.mUUID.setText("(Level : "+ String.valueOf(data.mPri)+")" );

                convertView.setTag(holder);
            }else{
                holder = (ViewHolder) convertView.getTag();
            }

            return convertView;

        }

        public void addItem(Drawable icon, String mTitle, String mDate, int mPri){
            ListData addInfo = null;
            addInfo = new ListData();
            // addInfo.mIcon = icon;
            addInfo.mTitle = mTitle;
            addInfo.mDate = mDate;
            addInfo.mPri = mPri;

            arrayList.add(addInfo);
        }

        public void remove(int position){
            arrayList.remove(position);
            dataChange();
        }

        public void sort(){
            Collections.sort(arrayList, ListData.ALPHA_COMPARATOR);
            dataChange();
        }

        public void dataChange(){
            mAdapter.notifyDataSetChanged();
        }



    }


}