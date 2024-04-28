package com.realrussia.game.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import com.realrussia.game.R;
import com.realrussia.game.core.Utils;
import androidx.appcompat.app.AppCompatActivity;
import org.ini4j.Wini;
import java.io.File;
import java.io.IOException;

import static com.realrussia.game.core.Config.GAME_PATH;

public class MainActivity extends AppCompatActivity
{
    private Button btn_play;
    private EditText nickname;

    protected void onCreate(Bundle savedInstanceState)
    {
        setContentView(R.layout.main_activity);
        Init(savedInstanceState);
        super.onCreate(savedInstanceState);
    }

    private boolean IsGameInstalled()
    {
        String GetGamePath = GAME_PATH + "texdb/gta3.img";
        File file = new File(GetGamePath);
        return file.exists();
    }

    private void Init(Bundle savedInstanceState)
    {
        btn_play = (Button) findViewById(R.id.btn_play);
        nickname = (EditText) findViewById(R.id.nick_edit);
        btn_play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View _view) {
                if(IsGameInstalled()) StartClient();
                else StartInstallGame();
            }
        });
//findViewById(R.id.nick_edit)
        ((EditText)nickname).setOnEditorActionListener(new EditText.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
            {
                if (actionId == EditorInfo.IME_ACTION_SEARCH || actionId == EditorInfo.IME_ACTION_DONE || event.getAction() == KeyEvent.ACTION_DOWN && event.getKeyCode() == KeyEvent.KEYCODE_ENTER)
                {
                    //if (!event.isShiftPressed()) // тут ебаная ошибка
                    //{
                    try {
                        File f = new File(GAME_PATH+"SAMP/crmp_settings.ini");
                        if(!f.exists()){ f.createNewFile();f.mkdirs();}
                        Wini w = new Wini(new File(GAME_PATH+"SAMP/crmp_settings.ini"));
                        w.put("client", "name", nickname.getText().toString());
                        w.store();
                        showMessage("Ваш новый никнейм успешно сохранен!");
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return false;
            }
        });
        InitLogic();
    }

    private void InitLogic() {
        try{
            Wini w = new Wini(new File(GAME_PATH+"SAMP/crmp_settings.ini"));
            nickname.setText(w.get("client", "name"));
            w.store();
            //CheckUpdateLauncher();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    private void StartInstallGame()
    {
        Intent intent = new Intent(this, com.realrussia.game.launcher.InstallGame.class);
        Utils.setType(0);
        intent.putExtras(getIntent());
        startActivity(intent);
    }

    private void StartClient()
    {
        Intent intent = new Intent(this, com.realrussia.game.core.GTASA.class);
        intent.putExtras(getIntent());
        System.out.println("StartActivity GTASA.class");
        startActivity(intent);
        finish();
    }
    private void showMessage(String _s) {
        Toast.makeText(getApplicationContext(), _s, Toast.LENGTH_SHORT).show();
    }
}
