package com.hazem.apps.libcurltest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button button;
    EditText editText;
    TextView textView;

    String response;
    Long start, end;

    Libcurl libcurl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button = (Button) findViewById(R.id.button);
        editText = (EditText) findViewById(R.id.edit_text);
        textView = (TextView) findViewById(R.id.text_view);
        button.setOnClickListener(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onClick(View v) {
        new Thread() {

            public void run() {
                libcurl = new Libcurl();
                response = "";
                String location = editText.getText().toString();
                location = location == null ? "" : location;
                start = System.currentTimeMillis();
                response = libcurl.requestPage(location, "");
                end = System.currentTimeMillis();

                runOnUiThread(
                        new Thread() {
                            public void run() {
                                Toast.makeText(MainActivity.this,""+(end-start)+" ms.",Toast.LENGTH_LONG).show();
                                textView.setText("Response:\n"+response);
                            }
                        }
                );

            }

        }.start();
    }
}