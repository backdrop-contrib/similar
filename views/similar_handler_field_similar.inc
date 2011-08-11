<?php

/**
 * @file
 * Defines the Similar entries field handler.
 */

/**
 * Defines the similar entries field handler.
 */
class similar_handler_field_similar extends views_handler_field {

  /**
   * Defines options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['similarity_format'] = array('default' => 0);
    $options['percent_suffix'] = array('default' => 0);
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['similarity_format'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => array(
        t('Show as raw similarity score'),
        t('Show as percentage'),
      ),
      '#default_value' => isset($this->options['similarity_format']) ? $this->options['similarity_format'] : 0,
      '#description' => t('Note that percentages are merely representative are are not necessarily accurate representations of the content similarity ratio.'),
    );
    $form['percent_suffix'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append % when showing percentage'),
      '#default_value' => !empty($this->options['percent_suffix']),
    );
  }

  /**
   * Empty query method.
   */
  public function query() {}

  /**
   * Renders output.
   */
  public function render($values) {
    if ($this->options['similarity_format'] == 0) {
      return drupal_substr($values->score, 0, 4);
    }
    elseif ($this->view->nids) {
      $output = round($values->score / count($this->view->nids));
      if (!empty($this->options['percent_suffix'])) {
        $output .= '%';
      }
      return $output;
    }
  }

}
