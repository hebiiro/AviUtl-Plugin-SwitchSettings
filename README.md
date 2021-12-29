# AviUtl プラグイン - 設定切り替え

* version 1.0.1 by 蛇色 - 2021/12/29 Detours を 4.0.1 に変更
* version 1.0.0 by 蛇色 - 2021/12/27 初版

起動時に設定ファイルがあるフォルダを選択できるようにします。プログラムの挙動から考えてセキュリティソフトからウィルス扱いされる可能性があります。そのときは例外に指定するとか自分でソースコードを改良してビルドするとか各自で対策をとってください。

## インストール

以下のファイルを AviUtl フォルダに入れてください。
* SwitchSettings.exe
* SwitchSettings.dll
* SwitchSettings (フォルダ)

## 使い方

1. SwitchSettings.exe を起動します。
2. フォルダ選択ダイアログが出るので設定ファイルがあるフォルダを選択します。
3. OK を押すと指定された設定で AviUtl が起動します。キャンセルを押すと AviUtl の起動もキャンセルされます。
4. 終了時に保存される設定も指定されたフォルダ内のファイルに保存されます。

* 今の所対応しているのは aviutl.ini、exedit.ini、デフォルト.cfg だけです。

## サンプル 1

1. SwitchSettings.exe を起動します。
2. フォルダ選択ダイアログが出るので "テスト用" フォルダを選択します。
3. AviUtl が起動すると、フィルタウィンドウがいくつか開いた状態で出てくるはずです。
4. これは "テスト用" フォルダの aviutl.ini を読み込んだからです。

## サンプル 2

1. SwitchSettings.exe を起動します。
2. フォルダ選択ダイアログが出るので "サイズ変更 1920x1080 FPS 1_2" フォルダを選択します。
3. AviUtl が起動すると、サイズ変更が 1920x1080、フレームレートが 1/2 に設定されているはずです。
4. これは "サイズ変更 1920x1080 FPS 1_2" フォルダの デフォルト.cfg を読み込んだからです。

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r8 https://www.nicovideo.jp/watch/sm39491708
* (共存確認) eclipse_fast 1.00 https://www.nicovideo.jp/watch/sm39756003
* (共存確認) 爆速プラグイン 1.06 https://www.nicovideo.jp/watch/sm39679253
