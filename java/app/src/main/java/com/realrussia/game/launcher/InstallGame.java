package com.realrussia.game.launcher;

import android.Manifest;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.FileProvider;

import com.realrussia.game.R;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.HttpURLConnection;
import java.net.URL;

import com.realrussia.game.core.Utils;
import com.tuyenmonkey.mkloader.model.Line;

import static android.widget.Toast.LENGTH_LONG;
import static com.realrussia.game.core.Config.APP_PATH;
import static com.realrussia.game.core.Config.GAME_PATH;
import static com.realrussia.game.core.Config.PATH_DOWNLOADS;
import static com.realrussia.game.core.Config.URL_CLIENT;
import static com.realrussia.game.core.Config.URL_FILES;
import static com.realrussia.game.core.Config.URL_FILES_UPDATE;

public class InstallGame extends AppCompatActivity {
    private TextView loading;
    private ProgressBar progressloading;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.game_install);
        initialize(savedInstanceState);
        if (Build.VERSION.SDK_INT >= 23) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED
                    || checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_DENIED) {
                requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1000);
            } else {
                InstallGame(Utils.getType());
            }
        } else {
            InstallGame(Utils.getType());
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 1000) {
            InstallGame(Utils.getType());
        }
    }

    public static void writeFile(String path, String str) {
        File file = new File(path);

        try {
            if (!file.exists())
                file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        FileWriter fileWriter = null;

        try {
            fileWriter = new FileWriter(new File(path), false);
            fileWriter.write(str);
            fileWriter.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fileWriter != null)
                    fileWriter.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void writeLog(Exception e){
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        e.printStackTrace(pw);
        String text = sw.toString();
        writeFile(APP_PATH+"/log.txt", text);
    }

    private void StartGame()
    {
        Intent intent = new Intent(this, com.realrussia.game.core.GTASA.class);
        intent.putExtras(getIntent());
        startActivity(intent);
        finish();
    }

    @Override
    public void onStop ()
    {
      //  mediaPlayer.stop();
        super.onStop();
    }

    private void InstallGame(int type)
    {
        switch (type) {
            case 0:
            {
                File dir = new File(PATH_DOWNLOADS);
                if (!dir.exists()) dir.mkdirs();
                final DownloadTask downloadTask = new DownloadTask(this);
                downloadTask.execute(URL_FILES);
                break;
            }
            case 1:
            {
                // Install UPDATE APK
                File dir = new File(PATH_DOWNLOADS);
                if (!dir.exists()) dir.mkdirs();
                final DownloadTask downloadTask = new DownloadTask(this);
                downloadTask.execute(URL_CLIENT);
                break;
            }
            case 2:
            {
                File dir = new File(PATH_DOWNLOADS);
                if (!dir.exists()) dir.mkdirs();
                final DownloadTask downloadTask = new DownloadTask(this);
                downloadTask.execute(URL_FILES_UPDATE);
                break;
            }
        }
    }

    private void initialize(Bundle savedInstanceState) {
        loading = (TextView) findViewById(R.id.downloadinfo);
        progressloading = (ProgressBar) findViewById(R.id.downloadingfiles);
    }

    private void UninstallFilesGame() {
        try {
            String command = "rm -r " + GAME_PATH;
            Process process = Runtime.getRuntime().exec("su");
            DataOutputStream os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(command + "\n");
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
        } catch (Exception e) {
            System.out.println(e);
        }

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Файлы игры удалены").
                setMessage("Файлы игры успешно удалены с вашего телефона!")
                //.setIcon(R.drawable.)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }});
    }

    public void UnZipZip()
    {
        final UnZipTask unzipTask = new UnZipTask(this);
        if(Utils.getType() == 0) unzipTask.execute(PATH_DOWNLOADS+"game.zip", GAME_PATH);
        else unzipTask.execute(PATH_DOWNLOADS+"upd.zip", GAME_PATH);
    }

    private void InstallAPK(final String apkname) {
        //try {
        //    File file = new File(PATH_DOWNLOADS, apkname + ".apk");
       //     Intent intent;
       //    if (file.exists()) {
       //        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
         //          intent = new Intent(Intent.ACTION_INSTALL_PACKAGE);
          //          intent.setData(apkUri);
          //          intent.setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
          //     } else {
            //        Uri apkUri = Uri.fromFile(file);
           //         intent = new Intent(Intent.ACTION_VIEW);
           //         intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
           //         intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
          //      }
         /////       startActivity(intent);
          //      finish();
          //  }
       //     else showMessage("Ошибка установки: файл не найден");
     //   } catch (Exception e) {
      //      Log.e("InstallAPK", "Ошибка установки:" + e.getMessage());
     //       writeLog(e);
    //   }
    }

    public void showMessage(String _s) {
        Toast.makeText(getApplicationContext(), _s, Toast.LENGTH_LONG).show();
    }

    private class DownloadTask extends AsyncTask<String, Integer, String> {

        private Context context;
        private PowerManager.WakeLock mWakeLock;

        public DownloadTask(Context context) {
            this.context = context;
        }

        @Override
        protected String doInBackground(String... sUrl) { // doInAnal
            InputStream input = null;
            OutputStream output = null;
            HttpURLConnection connection = null;
            try {
                URL url = new URL(sUrl[0]);
                connection = (HttpURLConnection) url.openConnection();
                connection.connect();
                if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
                    return "Server returned HTTP " + connection.getResponseCode()
                            + " " + connection.getResponseMessage();
                }
                int fileLength = connection.getContentLength();
                input = connection.getInputStream();
                if(Utils.getType() == 0) {
                    File file = new File(PATH_DOWNLOADS, "game.zip");
                    if (file.exists()) file.delete();
                    output = new FileOutputStream(PATH_DOWNLOADS + "game.zip");
                }
                else if(Utils.getType() == 1) {
                    File file = new File(PATH_DOWNLOADS, "launcher.apk");
                    if (file.exists()) file.delete();
                    output = new FileOutputStream(PATH_DOWNLOADS + "launcher.apk");
                }
                else if(Utils.getType() == 2) {
                    File file = new File(PATH_DOWNLOADS, "upd.zip");
                    if (file.exists()) file.delete();
                    output = new FileOutputStream(PATH_DOWNLOADS + "upd.zip");
                }
                byte data[] = new byte[4096];
                long total = 0;
                int count;
                while ((count = input.read(data)) != -1) {
                    if (isCancelled()) {
                        input.close();
                        return null;
                    }
                    total += count;
                    if (fileLength > 0)
                        publishProgress((int) (total * 100 / fileLength));
                    output.write(data, 0, count);
                }
            } catch (Exception e) {
                return e.toString();
            } finally {
                try {
                    if (output != null)
                        output.close();
                    if (input != null)
                        input.close();
                } catch (IOException ignored) {
                }

                if (connection != null)
                    connection.disconnect();
            }
            return null;
        }

        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
            PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, getClass().getName());
            mWakeLock.acquire();
            progressloading.setVisibility(View.VISIBLE);
            progressloading.setIndeterminate(false);
            progressloading.setMax(100);
        }

        @Override
        protected void onProgressUpdate(Integer... progress)
        {
            super.onProgressUpdate(progress);
            if(Utils.getType() == 0) loading.setText("Идет загрузка файлов...                  "+progress[0] + "%");
            if(Utils.getType() == 1) loading.setText("Идет загрузка лаунчера...               "+progress[0] + "%");
            if(Utils.getType() == 2) loading.setText("Идет загрузка доп.файлов...            "+progress[0] + "%");
            progressloading.setProgress(progress[0]);
        }

        @Override
        protected void onPostExecute(String result) {
            mWakeLock.release();
            loading.setVisibility(View.INVISIBLE);
            progressloading.setVisibility(View.INVISIBLE);
            if(Utils.getType() == 0 || Utils.getType() == 2)
            {
                if (result != null) Toast.makeText(context, "Ошибка загрузки: " + result, LENGTH_LONG).show();
                else Toast.makeText(context, "Файлы загружены.", Toast.LENGTH_SHORT).show();
                UnZipZip();
            }
            if(Utils.getType() == 1)
            {
                showMessage("Подтвердите установку");
                InstallAPK("launcher");
            }
        }
    }

    private class UnZipTask extends AsyncTask <String, Integer, String> {
        private Context context;

        public UnZipTask(Context context) {
            this.context = context;
        }
        @Override
        protected String doInBackground(String... params) {
            String filePath = params[0];
            String destinationPath = params[1];
            try {
                //java.io.File outdir = new java.io.File(destinationPath);
                File outdir = new File(destinationPath);
                if(outdir.exists()) outdir.delete();
                if(!outdir.exists()) {
                    outdir.mkdirs();
                    outdir.createNewFile();
                }
                File file = new File(filePath);
                if(!file.exists()) System.out.println("Pizda");
                java.util.zip.ZipInputStream zin = new java.util.zip.ZipInputStream(new java.io.FileInputStream(filePath));
                java.util.zip.ZipEntry entry;
                String name, dir;
                while ((entry = zin.getNextEntry()) != null) {
                    name = entry.getName();
                    if (entry.isDirectory()) {
                        mkdirs(outdir, name);
                        continue;
                    }
                    dir = dirpart(name);
                    if (dir != null)
                        mkdirs(outdir, dir);
                    byte[] buffer = new byte[4096];
                    java.io.BufferedOutputStream out = new java.io.BufferedOutputStream(new java.io.FileOutputStream(new java.io.File(outdir, name)));
                    int count = -1;
                    int publishprogress = 0;
                    long length = file.length();
                    while ((count = zin.read(buffer)) != -1) {
                        out.write(buffer, 0, count);
                        publishprogress += count;
                        publishProgress((int)(publishprogress / 100/* / length*/));
                    }
                    out.close();
                }
                zin.close();
            } catch (java.io.IOException e) {
                e.printStackTrace();
                writeLog(e);
            }
            return null;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            loading.setVisibility(View.VISIBLE);
            progressloading.setVisibility(View.VISIBLE);
            progressloading.setIndeterminate(false);
            progressloading.setMax(100);
        }

        @Override
        protected void onProgressUpdate(Integer... progress) {
            super.onProgressUpdate(progress);
            loading.setText("Идет распаковка файлов...");
            progressloading.setProgress(progress[0]);
        }

        @Override
        protected void onPostExecute(String result) {
            loading.setVisibility(View.INVISIBLE);
            progressloading.setVisibility(View.INVISIBLE);
            if (result != null)
                Toast.makeText(context, "Ошибка распаковки: " + result, LENGTH_LONG).show();
            else
                Toast.makeText(context, "Файлы распакованы.", Toast.LENGTH_SHORT).show();

            Toast.makeText(context, "Игра установлена. Игра запускается....", Toast.LENGTH_SHORT).show();
            StartGame();
        }

        private void mkdirs(java.io.File outdir, String path)
        {
            java.io.File d = new java.io.File(outdir, path);
            if(!d.exists())
                d.mkdirs();
        }

        private String dirpart(String name)
        {
            int s = name.lastIndexOf(java.io.File.separatorChar);
            return s == -1 ? null : name.substring(0, s);
        }
    }
}
