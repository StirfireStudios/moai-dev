  // Amazon Game Circle Client - BEGIN
  private com.amazon.ags.api.AmazonGamesClient amazongamecircleClient;

  com.amazon.ags.api.AmazonGamesCallback amazonGameCircleCallback = new com.amazon.ags.api.AmazonGamesCallback() {
    @Override
    public void onServiceNotReady(com.amazon.ags.api.AmazonGamesStatus status) {
        Moai.AKUDisableHusky();
    }
    @Override
    public void onServiceReady(com.amazon.ags.api.AmazonGamesClient amazonGamesClient) {
        amazongamecircleClient = amazonGamesClient;
        Moai.AKUEnableHusky();
    }
  };
  // Amazon Game Circle Client - END
